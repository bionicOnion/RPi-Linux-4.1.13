/* This file implements a demonstration syscall for an OS course. This is a
*  horrible function and your kernel should not include it. It sets the
*  userid of calling process to 0, giving it root permission.
*/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>

//Define a system call implementation that takes no arguments
SYSCALL_DEFINE0( badcall ){

	//Get the current user's namespace
	struct user_namespace *ns = current_user_ns();
	struct cred *new;

	//Create a kernelspace user id and group id of zero
	kuid_t kuid = make_kuid(ns, 0);
	kgid_t kgid = make_kgid(ns, 0);

	//Check that the userspace id is valid
	if(!uid_valid(kuid)) {
		return -EINVAL;
	}

	//Create a new user credentials set
	new = prepare_creds();

	//Make sure the new credentials set was constructed properly
	if(new != NULL) {
		//Set the normal, effective, session, and filesystem
		// user ID and group ID to zero
		new->uid = kuid;
		new->gid = kgid;
		new->euid = kuid;
		new->egid = kgid;
		new->suid = kuid;
		new->sgid = kgid;
		new->fsuid = kuid;
		new->fsgid = kgid;

		//Commit those credentials and return result
		return commit_creds(new);
	} else {
		//We didnt' have enough space to create new credentials
		abort_creds(new);
		return -ENOMEM;
	}
	
}

//End of file
