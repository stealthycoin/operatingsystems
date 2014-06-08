/* CREATED -- June 8, 2014 */
#include "fs.h"
#include <fcntl.h>
#include <unistd.h>
#include <minix/com.h>
#include <minix/u64.h>
#include "file.h"
#include "fproc.h"
#include "param.h"
#include <dirent.h>
#include <assert.h>
#include <minix/vfsif.h>
#include "vnode.h"
#include "vmnt.h"


/*===========================================================================*
 *				do_metar					     *
 *
 *	This function calls meta_read_write with the READING flag
 *===========================================================================*/
PUBLIC int do_metar()
{
  return(meta_read_write(READING));
}

/*===========================================================================*
 *				do_metaw					     *
 *
 *	This function calls meta_read_write with the WRITING flag
 *===========================================================================*/
PUBLIC int do_metaw()
{
  return(meta_read_write(WRITING));
}

/*===========================================================================*
 *				meta_read_write				     *
 *
 *	This function unpacks the message from our sys calls metar() / metaw()
 *	and makes the appropriate call to req_metareadwrite().
 *
 *===========================================================================*/
PUBLIC int meta_read_write(rw_flag)
int rw_flag;			/* READING or WRITING */
{
/* Perform read(fd, buffer, nbytes) or write(fd, buffer, nbytes) call. */
  register struct filp *f;
  register struct vnode *vp;
  unsigned int cum_io_incr;
  int r;

  /* If the file descriptor is valid, get the vnode, size and mode. */
  if (m_in.nbytes < 0) return(EINVAL);
  if ((f = get_filp(m_in.fd)) == NULL) return(err_code);
  if (m_in.nbytes == 0)
	return(0);	/* so char special files need not check for 0*/

  vp = f->filp_vno;
  r = OK;

  /* Issue request */
  r = req_metareadwrite(vp->v_fs_e, vp->v_inode_nr, rw_flag, who_e,
  		  m_in.buffer, m_in.nbytes, &cum_io_incr);

  if (r == OK) return(cum_io_incr);
  return(r);
}

