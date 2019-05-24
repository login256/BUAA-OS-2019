#include "lib.h"

int flag[256];

void lsdir(char*, char*);
void ls1(char*, u_int, u_int, char*);

void
ls(char *path, char *prefix)
{
	int r;
	struct Stat st;

	if ((r=stat(path, &st)) < 0)
		user_panic("stat %s: %e", path, r);
	if (st.st_isdir && !flag['d'])
		lsdir(path, prefix);
	else
		ls1(0, st.st_isdir, st.st_size, path);
}

void
lsdir(char *path, char *prefix)
{
	int fd, n;
	struct File f;

	if ((fd = open(path, O_RDONLY)) < 0)
		user_panic("open %s: %e", path, fd);
	while ((n = readn(fd, &f, sizeof f)) == sizeof f)
		if (f.f_name[0])
			ls1(prefix, f.f_type==FTYPE_DIR, f.f_size, f.f_name);
	if (n > 0)
		user_panic("short read in directory %s", path);
	if (n < 0)
		user_panic("error reading directory %s: %e", path, n);
}

void
ls1(char *prefix, u_int isdir, u_int size, char *name)
{
	char *sep;

	if(flag['l'])
		fwritef(1, "%11d %c ", size, isdir ? 'd' : '-');
	if(prefix) {
		if (prefix[0] && prefix[strlen(prefix)-1] != '/')
			sep = "/";
		else
			sep = "";
		fwritef(1, "%s%s", prefix, sep);
	}
	fwritef(1, "%s", name);
	if(flag['F'] && isdir)
		fwritef(1, "/");
	fwritef(1, " ");
}

void
usage(void)
{
	fwritef(1, "usage: ls [-dFl] [file...]\n");
	exit();
}

void
umain(int argc, char **argv)
{
	int i;

	ARGBEGIN{
	default:
		usage();
	case 'd':
	case 'F':
	case 'l':
		flag[(u_char)ARGC()]++;
		break;
	}ARGEND

	if (argc == 0)
		ls("/", "");
	else {
		for (i=0; i<argc; i++)
			ls(argv[i], argv[i]);
	}
}


