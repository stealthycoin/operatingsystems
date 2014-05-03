/* This file contains the scheduling policy for SCHED
 *
 * The entry points are:
 *   do_noquantum:        Called on behalf of process' that run out of quantum
 *   do_start_scheduling  Request to start scheduling a proc
 *   do_stop_scheduling   Request to stop scheduling a proc
 *   do_nice		          Request to change the nice level on a proc
 *   init_scheduling      Called from main.c to set up/prepare scheduling
 */
#include "sched.h"
#include "schedproc.h"
#include <assert.h>
#include <minix/com.h>
#include <machine/archtypes.h>
#include "kernel/proc.h" /* for queue constants */

PRIVATE timer_t sched_timer;
PRIVATE unsigned balance_timeout;

unsigned N_tix = 0;

#define BALANCE_TIMEOUT	1 /* how often to balance queues in seconds */

FORWARD _PROTOTYPE( int schedule_process, (struct schedproc * rmp)	);
FORWARD _PROTOTYPE( void balance_queues, (struct timer *tp)		);

#define DEFAULT_USER_TIME_SLICE 100
#define MAX_TICKETS 100

/*===========================================================================*
 *				do_noquantum				     *
 *===========================================================================*/

PUBLIC int do_noquantum(message *m_ptr)
{
  register struct schedproc *rmp;
  int rv, proc_nr_n;

  if (sched_isokendpt(m_ptr->m_source, &proc_nr_n) != OK) {
    printf("SCHED: WARNING: got an invalid endpoint in OOQ msg %u.\n",
	   m_ptr->m_source);
    return EBADEPT;
  }

  rmp = &schedproc[proc_nr_n];
  if (rmp->priority < MIN_USER_Q) {
    /* Dynamic Scheduler */
    if (DYNAMIC == 1 && rmp->priority >= MAX_USER_Q && rmp->n_tix > 1) {
      rmp->n_tix -= 1;
      N_tix -= 1;
      printf("Took a ticket; remaining: %d\n", rmp->n_tix);
    }
    if (rmp->priority >= MAX_USER_Q)
      rmp->priority = MIN_USER_Q;
    else if (rmp->priority < MAX_USER_Q - 1)
      rmp->priority += 1; /* lower priority */
  }
	
  if ((rv = schedule_process(rmp)) != OK) {
    return rv;
  }
  
  if (rmp->priority >= MAX_USER_Q) {
    if ((rv = lottery_winner()) != OK) {
      printf("Sched: lottery failed, error of type %d.\n", rv);
      return rv;
    }
  }

  return OK;
}

/*===========================================================================*
 *				do_stop_scheduling			     *
 *===========================================================================*/
PUBLIC int do_stop_scheduling(message *m_ptr)
{
  register struct schedproc *rmp;
  int rv, proc_nr_n;

  /* check who can send you requests */
  if (!accept_message(m_ptr))
    return EPERM;

  if (sched_isokendpt(m_ptr->SCHEDULING_ENDPOINT, &proc_nr_n) != OK) {
    printf("SCHED: WARNING: got an invalid endpoint in OOQ msg "
	   "%ld\n", m_ptr->SCHEDULING_ENDPOINT);
    return EBADEPT;
  }

  rmp = &schedproc[proc_nr_n];
  rmp->flags = 0; /*&= ~IN_USE;*/
  N_tix -= rmp->n_tix;

  return OK;
}

/*===========================================================================*
 *				do_start_scheduling			     *
 *===========================================================================*/
PUBLIC int do_start_scheduling(message *m_ptr)
{
  register struct schedproc *rmp;
  int rv, proc_nr_n, parent_nr_n, nice;
  /* we can handle two kinds of messages here */
  assert(m_ptr->m_type == SCHEDULING_START || 
	 m_ptr->m_type == SCHEDULING_INHERIT);

  /* check who can send you requests */
  if (!accept_message(m_ptr))
    return EPERM;

  /* Resolve endpoint to proc slot. */
  if ((rv = sched_isemtyendpt(m_ptr->SCHEDULING_ENDPOINT, &proc_nr_n))
      != OK) {
    return rv;
  }
  rmp = &schedproc[proc_nr_n];

  /* Populate process slot */
  rmp->endpoint     = m_ptr->SCHEDULING_ENDPOINT;
  rmp->parent       = m_ptr->SCHEDULING_PARENT;
  rmp->max_priority = (unsigned) m_ptr->SCHEDULING_MAXPRIO;
  if (rmp->max_priority >= NR_SCHED_QUEUES) {
    return EINVAL;
  }
	
  switch (m_ptr->m_type) {

  case SCHEDULING_START:
    /* We have a special case here for system processes, for which
     * quanum and priority are set explicitly rather than inherited 
     * from the parent */
    rmp->priority   = rmp->max_priority;
    rmp->time_slice = (unsigned) m_ptr->SCHEDULING_QUANTUM;
    break;
		
  case SCHEDULING_INHERIT:
    /* Inherit current priority and time slice from parent. Since there
     * is currently only one scheduler scheduling the whole system, this
     * value is local and we assert that the parent endpoint is valid */
    if ((rv = sched_isokendpt(m_ptr->SCHEDULING_PARENT,
			      &parent_nr_n)) != OK)
      return rv;

    /* rmp->priority = schedproc[parent_nr_n].priority;
    if (rmp->priority >= MAX_USER_Q) */ 
    rmp->priority = MIN_USER_Q;
    rmp->time_slice = schedproc[parent_nr_n].time_slice;
    rmp->n_tix = 20;
    N_tix += 20;
    break;
		
  default: 
    /* not reachable */
    assert(0);
  }

  /* Take over scheduling the process. The kernel reply message populates
   * the processes current priority and its time slice */
  if ((rv = sys_schedctl(0, rmp->endpoint, 0, 0)) != OK) {
    printf("Sched: Error taking over scheduling for %d, kernel said %d\n",
	   rmp->endpoint, rv);
    return rv;
  }
  rmp->flags = IN_USE;

  /* Schedule the process, giving it some quantum */
  if ((rv = schedule_process(rmp)) != OK) {
    printf("Sched: Error while scheduling process, kernel replied %d\n",
     rv);
    return rv;
  }

  /* Mark ourselves as the new scheduler.
   * By default, processes are scheduled by the parents scheduler. In case
   * this scheduler would want to delegate scheduling to another
   * scheduler, it could do so and then write the endpoint of that
   * scheduler into SCHEDULING_SCHEDULER
   */

  m_ptr->SCHEDULING_SCHEDULER = SCHED_PROC_NR;
  return OK;
}

/*===========================================================================*
 *				do_nice					     *
 *===========================================================================*/
PUBLIC int do_nice(message *m_ptr)
{
  struct schedproc *rmp;
  int rv;
  int proc_nr_n, nice;
  unsigned old_q, old_max_q;

  /* check who can send you requests */
  if (!accept_message(m_ptr))
    return EPERM;

  if (sched_isokendpt(m_ptr->SCHEDULING_ENDPOINT, &proc_nr_n) != OK) {
    printf("SCHED: WARNING: got an invalid endpoint in OOQ msg "
	   "%ld\n", m_ptr->SCHEDULING_ENDPOINT);
    return EBADEPT;
  }

  rmp = &schedproc[proc_nr_n];
  nice = m_ptr->SCHEDULING_MAXPRIO;

  /* Store old values, in case we need to roll back the changes */
  old_q     = rmp->priority;
  old_max_q = rmp->max_priority;

  /* Update the proc entry and reschedule the process */
  if (rmp->priority < MAX_USER_Q)
    rmp->max_priority = rmp->priority = 7;
  else {
    rmp->priority = MIN_USER_Q;
    set_tix(rmp, nice);
  }

  printf("Total tickets: %d\n Process tickets: %d\n", N_tix, rmp->n_tix);

  if ((rv = schedule_process(rmp)) != OK) {
    /* Something went wrong when rescheduling the process, roll
     * back the changes to proc struct */
    rmp->priority     = old_q;
    rmp->max_priority = old_max_q;
  }

  return rv;
}

/*===========================================================================*
 *				schedule_process			     *
 *===========================================================================*/
PRIVATE int schedule_process(struct schedproc * rmp)
{
  int rv;
  if ((rv = sys_schedule(rmp->endpoint, rmp->priority,
			 rmp->time_slice)) != OK) {
    printf("SCHED: An error occurred when trying to schedule %d: %d\n",
	   rmp->endpoint, rv);
  }

  return rv;
}


/*===========================================================================*
 *				start_scheduling			     *
 *===========================================================================*/

PUBLIC void init_scheduling(void)
{
  u64_t random;
  balance_timeout = BALANCE_TIMEOUT * sys_hz();
  init_timer(&sched_timer);
  set_timer(&sched_timer, balance_timeout, balance_queues, 0);
  read_tsc_64(&random);
  srand((unsigned) random.lo);
}

/*===========================================================================*
 *				balance_queues				     *
 *===========================================================================*/

/* This function in called every 100 ticks to rebalance the queues. The current
 * scheduler bumps processes down one priority when ever they run out of
 * quantum. This function will find all proccesses that have been bumped down,
 * and pulls them back up. This default policy will soon be changed.
 */
PRIVATE void balance_queues(struct timer *tp)
{
  struct schedproc *rmp;
  int proc_nr;
  int rv;
  for (proc_nr=0, rmp=schedproc; proc_nr < NR_PROCS; proc_nr++, rmp++) {
    if (rmp->priority < MAX_USER_Q) { /* system process */
      if (rmp->flags & IN_USE) {
        if (rmp->priority > rmp->max_priority) {
          rmp->priority -= 1; /* increase priority */
          schedule_process(rmp);
        }
      }
    } else { /* user process */
      if (rmp->flags & IN_USE) {
        if (rmp->priority == MAX_USER_Q) {
          rmp->priority = MIN_USER_Q;
          schedule_process(rmp);
        }
      }
    }
  }
  set_timer(&sched_timer, balance_timeout, balance_queues, 0);
}

/*===========================================================================*
 *		                  		set_tix                             				     *
 *===========================================================================*/

/* This function adjusts the ticket field of the schedproc rmp by 
 * the requested amount, if possible. If adding the requested amount
 * of tickets would result in more than the max allowable, then this
 * procedures sets the ticket field to the max. If removing the requested
 * amount would result in less than 1 ticket, the ticket field is set to 1.
 */
void set_tix(struct schedproc *rmp, int new_q)
{
  if (rmp->n_tix + new_q <= MAX_TICKETS && rmp->n_tix + new_q > 0) {
    rmp->n_tix += new_q;
    N_tix += new_q;
  } else if (rmp->n_tix + new_q > MAX_TICKETS) {
    N_tix += MAX_TICKETS - rmp->n_tix;
    rmp->n_tix = MAX_TICKETS;
  } else {
    N_tix -= rmp->n_tix - 1;
    rmp->n_tix = 1;
  }
}

/*===========================================================================*
 *		                		lotterty_winner	                        			     *
 *===========================================================================*/

 /* Determines the lottery winner. 
  *
  */
int lottery_winner()
{
  int proc, winning_ticket;
  struct schedproc *rmp;

  winning_ticket = N_tix ? rand() % N_tix : 0;
  for (proc=0, rmp=schedproc; proc < NR_PROCS; proc++, rmp++) {
    if ((rmp->flags & IN_USE) && rmp->priority == MIN_USER_Q) {
      winning_ticket -= rmp->n_tix;
      if (winning_ticket <= 0) {
        rmp->priority = MAX_USER_Q;
        schedule_process(rmp);
        break;
      }
    }
  }

  return OK;
}
