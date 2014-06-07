/* This file contains the heart of the mechanism used to read (and write)
 * files.  Read and write requests are split up into chunks that do not cross
 * block boundaries.  Each chunk is then processed in turn.  Reads on special
 * files are also detected and handled.
 *
 * The entry points into this file are
 *   do_read:	 perform the READ system call by calling read_write
 *   do_getdents: read entries from a directory (GETDENTS)
 *   read_write: actually do the work of READ and WRITE
 *
 */

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
 *===========================================================================*/
PUBLIC int do_metar()
{
  return(meta_read_write(READING));
}

/*===========================================================================*
 *				do_metaw					     *
 *===========================================================================*/
PUBLIC int do_metaw()
{
  return(meta_read_write(WRITING));
}

/*===========================================================================*
 *				meta_read_write				     *
 *===========================================================================*/
PUBLIC int meta_read_write(rw_flag)
int rw_flag;			/* READING or WRITING */
{
/* Perform read(fd, buffer, nbytes) or write(fd, buffer, nbytes) call. */
  register struct filp *f;
  register struct vnode *vp;
  u64_t res_pos, new_pos;
  unsigned int cum_io, cum_io_incr, res_cum_io;
  int op, oflags, r, block_spec, char_spec;
  int regular;
  mode_t mode_word;

  /* If the file descriptor is valid, get the vnode, size and mode. */
  if (m_in.nbytes < 0) return(EINVAL);
  if ((f = get_filp(m_in.fd)) == NULL) return(err_code);
  if (((f->filp_mode) & (rw_flag == READING ? R_BIT : W_BIT)) == 0) {
	return(f->filp_mode == FILP_CLOSED ? EIO : EBADF);
  }
  if (m_in.nbytes == 0)
	return(0);	/* so char special files need not check for 0*/

  /* position = f->filp_pos; */
  oflags = f->filp_flags;
  vp = f->filp_vno;
  r = OK;
  cum_io = 0;

  mode_word = vp->v_mode & I_TYPE;
  regular = mode_word == I_REGULAR;
/*
  if ((char_spec = (mode_word == I_CHAR_SPECIAL ? 1 : 0))) {
	if (vp->v_sdev == NO_DEV)
		panic("read_write tries to read from character device NO_DEV");
  }

  if ((block_spec = (mode_word == I_BLOCK_SPECIAL ? 1 : 0))) {
	if (vp->v_sdev == NO_DEV)
		panic("read_write tries to read from block device NO_DEV");
  }
  */

  /* Issue request */
  r = req_metareadwrite(vp->v_fs_e, vp->v_inode_nr, rw_flag, who_e,
  		  m_in.buffer, m_in.nbytes, &cum_io_incr);

  if (r == OK) return(cum_io);
  return(r);
}

