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
	//fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL, 0) | O_NONBLOCK);
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

static char keystate[256] = { 0 };

void process_input()
{
	char c;

	std::fill(keystate, keystate + 256, 0);

	if (_kbhit())
	{
		c = _getch();
		std::cout << "input : " << c << '\n';
		unsigned char uc = (unsigned char)c;
		keystate[uc] = 1;
	}
}

int is_key_pressed(char key)
{
	return keystate[(unsigned char)key];
}

int main()
{
	init_terminal();
	while (true)
	{
		process_input();
		Sleep(20);
		if (is_key_pressed('q'))
		{
			exit(0);
		}
	}
	restore_terminal();

	return 0;
}