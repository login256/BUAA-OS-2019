/* simple command line parser from Plan 9 -- you are not expected to understand this */
/* see http://plan9.bell-labs.com/magic/man2html/2/arg */


#define _ARGSET(x) (x)=0
#define _ARGUSED(x) if(x){}else

#define	ARGBEGIN	for((argv?0:(argv=(void*)&argc)),\
						argv++,argc--;\
						argv[0] && argv[0][0]=='-' && argv[0][1];\
						argc--, argv++) {\
	char *_args, *_argt;\
	char _argc;\
	_args = &argv[0][1];\
	if(_args[0]=='-' && _args[1]==0){\
		argc--; argv++; break;\
	}\
	_argc = 0;\
	while(*_args && (_argc = *_args++))\
		switch(_argc)
#define	ARGEND		_ARGSET(_argt);_ARGUSED(_argt);_ARGUSED(_argc);_ARGUSED(_args);}_ARGUSED(argv);_ARGUSED(argc);
#define	ARGF()		(_argt=_args, _args="",\
					 (*_argt? _argt: argv[1]? (argc--, *++argv): 0))
#define	EARGF(x)	(_argt=_args, _args="",\
					 (*_argt? _argt: argv[1]? (argc--, *++argv): ((x), abort(), (char*)0)))

#define	ARGC()		_argc
