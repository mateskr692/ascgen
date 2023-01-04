#include "program.h"
#include "result.h"

int main(int argc, char* argv[])
{
	struct mainargs arg =
	{
		#if _DEBUG
		.argv = (char*[]){ "some path", "-i", "C:\\Users\\Skrzelik\\Desktop\\in¿\\input.txt"},
		.argc = 3,
		#else
		.argv = argv,
		.argc = argc,
		#endif
	};
	
	struct result res = execute(arg);
	if (res.is_error)
	{
		printf(res.message);
		return 1;
	}

	return 0;
}

