#include <fcntl.h>

#include <cstdio>
#include <cstdint>
#include <iostream>

#include <io.h>
#include <process.h>
#include <direct.h>
#include <conio.h>
#include <Windows.h>

static DWORD old_mode;

void init_terminal()
{
	// windows
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(hStdin, &old_mode);
	DWORD new_mode = old_mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
	SetConsoleMode(hStdin, new_mode);
	fflush(stdout);

	// linux/unix
	//tcgetattr(STDIN_FILENO, &old_termios);
	//new_termios = old_termios;
	//new_termios.c_lfflag &= ~(ICANON | ECHO);
	//tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);
	//fflush(stdout);
}

void restore_terminal()
{
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	SetConsoleMode(hStdin, old_mode);
	std::cout << "terminal restored";

	// linux/unix
	//tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
}

void process_input()
{
	char c;
	if (_kbhit())
	{
		c = _getch();
		std::cout << "input : " << c << '\n';
	}
}

int main()
{
	init_terminal();
	while (true)
	{
		process_input();
		Sleep(20);
	}
	restore_terminal();

	return 0;
}