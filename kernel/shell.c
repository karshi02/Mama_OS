#include "shell.h"
#include "vga.h"

#define CMD_BUF_SIZE 128

static char cmd_buf[CMD_BUF_SIZE];
static int  cmd_len = 0;

static int str_eq(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) return 0;
        a++; b++;
    }
    return *a == *b;
}

static int str_starts(const char *s, const char *prefix) {
    while (*prefix) {
        if (*s != *prefix) return 0;
        s++; prefix++;
    }
    return 1;
}

static void cmd_help(void) {
    vga_print("Available commands:\n");
    vga_print("  help       - show this message\n");
    vga_print("  clear      - clear screen\n");
    vga_print("  echo <msg> - print message\n");
    vga_print("  version    - show OS version\n");
}

static void cmd_version(void) {
    vga_print("MyOS v0.1-alpha\n");
    vga_print("Built with love and Assembly\n");
}

static void shell_exec(void) {
    if (cmd_len == 0) return;
    cmd_buf[cmd_len] = '\0';

    if (str_eq(cmd_buf, "help")) {
        cmd_help();
    } else if (str_eq(cmd_buf, "clear")) {
        vga_clear();
    } else if (str_eq(cmd_buf, "version")) {
        cmd_version();
    } else if (str_starts(cmd_buf, "echo ")) {
        vga_print(cmd_buf + 5);
        vga_putchar('\n');
    } else {
        vga_print("Unknown command: ");
        vga_print(cmd_buf);
        vga_putchar('\n');
    }
}

static void shell_prompt(void) {
    vga_print("> ");
}

void shell_init(void) {
    cmd_len = 0;
    vga_print("MyOS Shell ready. Type 'help' for commands.\n");
    shell_prompt();
}

void shell_handle_char(char c) {
    if (c == '\n') {
        vga_putchar('\n');
        shell_exec();
        cmd_len = 0;
        shell_prompt();
    } else if (c == '\b') {
        if (cmd_len > 0) {
            cmd_len--;
            vga_putchar('\b');
        }
    } else {
        if (cmd_len < CMD_BUF_SIZE - 1) {
            cmd_buf[cmd_len++] = c;
            vga_putchar(c);
        }
    }
}
