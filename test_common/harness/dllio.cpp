#ifdef _WIN32
#include<Windows.h>
#endif

#include<stdio.h>
#include<stdarg.h>
#include<string.h>
#include<memory.h>
#include<malloc.h>
#include<sys/stat.h>
#include<io.h>
#include<exception>
#include<unordered_set>
#include<iostream>

#include "dllio_hooks.h"

///////////////////////////////////////////////////////////////////////////////////////////

extern "C" int main(int argc, char **argv);

extern "C"
{

	void *hook_ctx = nullptr;
	void (*stdout_hook)(void *ctx, const char *str) = nullptr;
    void (*stderr_hook)(void *ctx, const char *str) = nullptr;
    std::ostream *cout_stream = nullptr;
    std::ostream *cerr_stream = nullptr;
    //std::istream *cin_stream = nullptr;

	int dllexitcode = 0;
	int dllexited = 0;

	int strprintf(char **psz, const char *szFmt, va_list args)
	{
		int nLen;
		if ((nLen = vsnprintf(NULL, 0, szFmt, args)) >= 0)
		{
			char *szRes = (char*)realloc(*psz, nLen + 1);
			if (szRes)
				vsnprintf(*psz = szRes, nLen + 1, szFmt, args);
			else
				nLen = -1;
		}
		return nLen;
	}


	int dllprintf(const char * _Format, ...)
	{
		int ret;

		va_list args;
		va_start(args, _Format);

		if (stdout_hook)
		{
			char *str = nullptr;
			ret = strprintf(&str, _Format, args);
			stdout_hook(hook_ctx, str);
			free(str);
		}
		else
		{
			ret = vprintf(_Format, args);
		}

		va_end(args);
		return ret;
	}

	int dllvprintf(const char * _Format, va_list _ArgList)
	{
		int ret;

		if (stdout_hook)
		{
			char *str = nullptr;
			ret = strprintf(&str, _Format, _ArgList);
			stdout_hook(hook_ctx, str);
			free(str);
		}
		else
		{
			ret = vprintf(_Format, _ArgList);
		}

		return ret;
	}

	int dllfprintf(FILE *f, const char * _Format, ...)
	{
		int ret;

		if (stdout_hook && f == stdout)
		{
			va_list args;
			va_start(args, _Format);
			char *str = nullptr;
			ret = strprintf(&str, _Format, args);
			va_end(args);

			stdout_hook(hook_ctx, str);
			free(str);
		}
		else if (stderr_hook && f == stderr)
		{
			va_list args;
			va_start(args, _Format);
			char *str = nullptr;
			ret = strprintf(&str, _Format, args);
			va_end(args);

			stderr_hook(hook_ctx, str);
			free(str);
		}
		else
		{
			va_list args;
			va_start(args, _Format);
			ret = vfprintf(f, _Format, args);
			va_end(args);
		}


		return ret;
	}
	
	int dllvfprintf(FILE *f, const char * _Format, va_list _ArgList)
	{
		int ret;

		if (stdout_hook && f == stdout)
		{
			char *str = nullptr;
			ret = strprintf(&str, _Format, _ArgList);

			stdout_hook(hook_ctx, str);
			free(str);
		}
		else if (stderr_hook && f == stderr)
		{
			char *str = nullptr;
			ret = strprintf(&str, _Format, _ArgList);
			
			stderr_hook(hook_ctx, str);
			free(str);
		}
		else
		{
			ret = vfprintf(f, _Format, _ArgList);
		}

		return ret;
	}

	int dllfputs(const char *str, FILE *f)
	{
		if (stdout_hook && f == stdout)
		{
			stdout_hook(hook_ctx, str);
			return (int)strlen(str);
		}
		else if (stderr_hook && f == stderr)
		{
			stderr_hook(hook_ctx, str);
			return (int)strlen(str);
		}

		return fputs(str, f);
	}

	int dllputs(const char *str)
	{
		if (stdout_hook)
		{
			stdout_hook(hook_ctx, str);
			return (int)strlen(str);
		}
		return puts(str);
	}

	int dllputchar(int c)
	{
		if (stdout_hook)
		{
			char ch[2];
			ch[0] = c;
			ch[1] = '\0';
			stdout_hook(hook_ctx, ch);
			return 1;
		}
		return putchar(c);
	}

	int dllfputc(int c, FILE *f)
	{
		if (stdout_hook && f == stdout)
		{
			char ch[2];
			ch[0] = c;
			ch[1] = '\0';

			stdout_hook(hook_ctx, ch);
			return 1;
		}
		else if (stderr_hook && f == stderr)
		{
			char ch[2];
			ch[0] = c;
			ch[1] = '\0';

			stderr_hook(hook_ctx, ch);
			return 1;
		}

		return fputc(c, f);
	}

	void dllexit(int exitcode)
	{
		dllexitcode = exitcode;
		dllexited = 1;
		throw (int)1;
	}

	void dll_exit(int exitcode)
	{
		dllexit(exitcode);
	}

	int dllio_main(int argc, char **argv, struct DLLIO_HOOKS *hooks)
	{

//		int tmp = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
//		tmp |= _CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_CRT_DF | _CRTDBG_DELAY_FREE_MEM_DF | _CRTDBG_CHECK_EVERY_1024_DF;//|_CRTDBG_CHECK_ALWAYS_DF; //|_CRTDBG_LEAK_CHECK_DF;// | _CRTDBG_CHECK_EVERY_16_DF;
//		_CrtSetDbgFlag(tmp);

		try
		{
			if (hooks)
			{
				hook_ctx = hooks->ctx;
				stdout_hook = hooks->stdout_hook;
				stderr_hook = hooks->stderr_hook;
                // cin_stream = hooks->cin_stream;
                cout_stream = hooks->cout_stream; 
                cerr_stream = hooks->cerr_stream; 
                
                //std::cin.rdbuf(cin_stream->rdbuf());
                std::cout.rdbuf(cout_stream->rdbuf());
                std::cerr.rdbuf(cerr_stream->rdbuf());
            }

			int ret = main(argc, argv);

			return ret;
		}
		catch (...)
		{
return dllexitcode;
		}
	}

	void null_stdhook(void *, const char *)
	{
	}
}
