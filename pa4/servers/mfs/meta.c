#include "fs.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <minix/com.h>
#include <minix/u64.h>
#include "buf.h"
#include "inode.h"
#include "super.h"
#include <minix/vfsif.h>
#include <assert.h>


FORWARD _PROTOTYPE( int rw_metachunk, (struct inode *rip,
    size_t chunk, unsigned left, int rw_flag,
	cp_grant_id_t gid, unsigned buf_off, unsigned int block_size,
	int *completed)							);

/*===========================================================================*
 *				fs_metareadwrite				     *
 *===========================================================================*/
PUBLIC int fs_metareadwrite(void)
{
  int r, rw_flag;
  int regular;
  cp_grant_id_t gid;
  off_t f_size, bytes_left;
  unsigned int off, cum_io, block_size, chunk;
  mode_t mode_word;
  int completed;
  struct inode *rip;
  size_t nrbytes;
  
  r = OK;
  
  printf("Within fs_metareadwrite\n");

  /* Find the inode referred */
  if ((rip = find_inode(fs_dev, (ino_t) fs_m_in.REQ_INODE_NR)) == NULL)
	return(EINVAL);

  mode_word = rip->i_mode & I_TYPE;
  regular = (mode_word == I_REGULAR || mode_word == I_NAMED_PIPE);
  
  /* Determine blocksize */
  block_size = rip->i_sp->s_block_size;

  /* Get the values from the request message */ 
  rw_flag = (fs_m_in.m_type == REQ_READ ? READING : WRITING);
  gid = (cp_grant_id_t) fs_m_in.REQ_GRANT;
  nrbytes = (size_t) fs_m_in.REQ_NBYTES;
  
  rdwt_err = OK;		/* set to EIO if disk error occurs */
	      
  cum_io = 0;
  /* Split the transfer into chunks that don't span two blocks. */
  while (nrbytes > 0) {
	  chunk = min(nrbytes, block_size);

	  /* Read or write 'chunk' bytes. */
	  r = rw_metachunk(rip, chunk,
	  	       nrbytes, rw_flag, gid, cum_io, block_size, &completed);

      printf("returned from metachunk\n");

	  if (r != OK) break;	/* EOF reached */
	  if (rdwt_err < 0) break;

	  /* Update counters and pointers. */
	  nrbytes -= chunk;	/* bytes yet to be read */
	  cum_io += chunk;	/* bytes read so far */
  }

  if (rdwt_err != OK) r = rdwt_err;	/* check for disk error */
  if (rdwt_err == END_OF_FILE) r = OK;

  if (r == OK) {
	  if (rw_flag == READING) rip->i_update |= ATIME;
	  if (rw_flag == WRITING) rip->i_update |= CTIME | MTIME;
	  rip->i_dirt = DIRTY;		/* inode is thus now dirty */
  }
  
  fs_m_out.RES_NBYTES = cum_io;
  
  return(r);
}


/*===========================================================================*
 *				rw_metachunk				     *
 *===========================================================================*/
PRIVATE int rw_metachunk(rip, chunk, left, rw_flag, gid,
 buf_off, block_size, completed)
register struct inode *rip;	/* pointer to inode for file to be rd/wr */
unsigned int chunk;		/* number of bytes to read or write */
unsigned left;			/* max number of bytes wanted after position */
int rw_flag;			/* READING or WRITING */
cp_grant_id_t gid;		/* grant */
unsigned buf_off;		/* offset in grant */
unsigned int block_size;	/* block size of FS operating on */
int *completed;			/* number of bytes copied */
{
/* Read or write (part of) a block. */

  register struct buf *bp;
  register int r = OK;
  int n;
  block_t b;
  dev_t dev;

  *completed = 0;
  dev = rip->i_dev;


  if (rip->i_zone[9] == NO_ZONE) {
    rip->i_zone[9] = alloc_zone(rip->i_dev, rip->i_zone[9]);
    b = (block_t) rip->i_zone[9] << rip->i_sp->s_log_zone_size;
    if (b == NO_BLOCK)
        printf("no_zone ==> no_block\n");
  } else {
    b = (block_t) rip->i_zone[9] << rip->i_sp->s_log_zone_size;
  }


  if (b == NO_BLOCK) {
	if (rw_flag == READING) {
		/* Reading from a nonexistent block.  Must read as all zeros.*/
		bp = get_block(NO_DEV, NO_BLOCK, NORMAL);    /* get a buffer */
		zero_block(bp);
	} else {
		/* Writing to a nonexistent block. Create and enter in inode.*/
		bp = get_block(dev, b, NORMAL);
	}
  } else if (rw_flag == READING) {
    bp = get_block(dev, b, NORMAL);
  } else {
	/* Normally an existing block to be partially overwritten is first read
	 * in.  However, a full block need not be read in.  If it is already in
	 * the cache, acquire it, otherwise just acquire a free buffer.
	 */
	n = (chunk == block_size ? NO_READ : NORMAL);
	bp = get_block(dev, b, n);
  }

  /* In all cases, bp now points to a valid buffer. */
  if (bp == NULL) 
  	panic("bp not valid in rw_metachunk; this can't happen");
  
  if (rw_flag == WRITING && chunk != block_size) {
	zero_block(bp);
  }

  if (rw_flag == READING) {
	/* Copy a chunk from the block buffer to user space. */
	r = sys_safecopyto(VFS_PROC_NR, gid, (vir_bytes) buf_off,
			   (vir_bytes) (bp->b_data), (size_t) chunk, D);
    printf("Just read the following:\n");
    printf("%s\n", bp->b_data);
  } else {
	/* Copy a chunk from user space to the block buffer. */
	r = sys_safecopyfrom(VFS_PROC_NR, gid, (vir_bytes) buf_off,
			     (vir_bytes) (bp->b_data), (size_t) chunk, D);
	bp->b_dirt = DIRTY;
    printf("Just wrote the following:\n");
    printf("%s\n", bp->b_data);
  }
  
  n = (chunk == block_size ? FULL_DATA_BLOCK : PARTIAL_DATA_BLOCK);
  put_block(bp, n);

  return(r);
}
