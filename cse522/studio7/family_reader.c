#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>


static pid_t pid;


/*
 * The "foo" file where a static variable is read from and written to.
 */
static ssize_t show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{
	return sprintf(buf, "Unsupported functionality\n");
}

static ssize_t store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count)
{
	int ret = kstrtoint(buf, 10, &pid);
	if (ret < 0)
		return ret;

	pid* real_pid = find_vpid(pid);
	if (real_pid == NULL)
	{
		printk(KERN_WARNING "VPID translation failed\n");
		return count;
	}

	task_struct* pid_task = pid_task(real_pid, PIDTYPE_PID);
	if (real_pid == NULL)
	{
		printk(KERN_WARNING "PID task locating failed\n");
		return count;
	}

	// TODO finish step 5 here

	printk(KERN_INFO "PID logged: %d\n", pid);

	return count;
}

/* Sysfs attributes cannot be world-writable. */
static struct kobj_attribute pid_attribute =
	__ATTR(pid, 0664, show, store);

/*
 * Create a group of attributes so that we can create and destroy them all
 * at once.
 */
static struct attribute *attrs[] = {
	&pid_attribute.attr,
	NULL,	/* need to NULL terminate the list of attributes */
};

/*
 * An unnamed attribute group will put all of the attributes directly in
 * the kobject directory.  If we specify a name, a subdirectory will be
 * created for the attributes with the directory being the name of the
 * attribute group.
 */
static struct attribute_group attr_group = {
	.attrs = attrs,
};

static struct kobject *example_kobj;

static int __init example_init(void)
{
	int retval;

	/*
	 * Create a simple kobject with the name of "kobject_example",
	 * located under /sys/kernel/
	 *
	 * As this is a simple directory, no uevent will be sent to
	 * userspace.  That is why this function should not be used for
	 * any type of dynamic kobjects, where the name and number are
	 * not known ahead of time.
	 */
	example_kobj = kobject_create_and_add("fam_reader", kernel_kobj);
	if (!example_kobj)
		return -ENOMEM;

	/* Create the files associated with this kobject */
	retval = sysfs_create_group(example_kobj, &attr_group);
	if (retval)
		kobject_put(example_kobj);

	return retval;
}

static void __exit example_exit(void)
{
	kobject_put(example_kobj);
}

module_init(example_init);
module_exit(example_exit);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Greg Kroah-Hartman <greg@kroah.com>");
