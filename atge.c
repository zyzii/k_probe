#include <linux/module.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
#define log(a, ...) printk("[ %s : %.3d ] "a"\n", \
				__FUNCTION__, __LINE__,  ## __VA_ARGS__)

static int sb_set(struct super_block *sb, void *data)
{
	sb->s_fs_info = data;
	return set_anon_super(sb, NULL);
}

static int zfs_fill_super(struct super_block *sb, void *data, int silent)
{

	return 0;
}

static struct dentry *zfs_mount(struct file_system_type *fs_type, int flags,
			const char *name, void *data)
{
	struct super_block *sb;

	sb = sget(fs_type, NULL, sb_set, flags, NULL);
	if (IS_ERR(sb)) {
		err = PTR_ERR(sb);
		goto out_fail;
	}

	err = zfs_fill_super(sb, data, 1);
	log();
	return NULL;

out_fail:
	return ERR_PTR(err);
}

static void kill_zfs_super(struct super_block *s)
{
	log();
}

static struct file_system_type zfs_fs_type = {
	.name    = "zfs",
	.owner   = THIS_MODULE,
	.mount   = zfs_mount,
	.kill_sb = kill_zfs_super,
};
MODULE_ALIAS_FS("zfs");

static int __init zyzii_init(void)
{
	int err;

	log("---------------test fs------------------------");
	err = register_filesystem(&zfs_fs_type);
	return 0;
}

static void __exit zyzii_exit(void)
{
	unregister_filesystem(&zfs_fs_type);
	log("---------------- bye world ---------------------");
}

module_init(zyzii_init);
module_exit(zyzii_exit);
