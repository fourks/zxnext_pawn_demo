/*******************************************************************************
 * Stefan Bylund 2017
 *
 * String utility functions.
 ******************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stropts.h>
#include <ctype.h>
#include <rect.h>
#include <font/fzx.h>

#include "str_util.h"

char *str_lower_case(char *s)
{
    if (s != NULL)
    {
        uint16_t i;

        for (i = 0; s[i]; i++)
        {
            s[i] = tolower(s[i]);
        }
    }

    return s;
}

char *str_remove_cr(char *s)
{
    if ((s != NULL) && (strchr(s, '\r') != NULL))
    {
        char *src;
        char *dst;

        for (src = s, dst = s; *src != '\0'; src++)
        {
            *dst = *src;
            if (*dst != '\r')
            {
                dst++;
            }
        }

        *dst = '\0';
    }

    return s;
}

char *str_remove_trailing_lf(char *s)
{
    if ((s != NULL) && (strlen(s) > 0))
    {
        if (s[strlen(s) - 1] == '\n')
        {
            s[strlen(s) - 1] = '\0';
        }
    }

    return s;
}

char *str_word_wrap(FILE *out, char *s)
{
    int fd;
    struct fzx_font *font;
    struct r_Rect16 paper;
    uint16_t width;
    char *line;
    char *line_end;

    if ((out == NULL) || (s == NULL))
    {
        return s;
    }

    fd = fileno(out);
    if (fd < 0)
    {
        return s;
    }

    font = (struct fzx_font *) ioctl(fd, IOCTL_OTERM_FONT, -1);
    ioctl(fd, IOCTL_OTERM_FZX_GET_PAPER_RECT, &paper);
    width = paper.width - ioctl(fd, IOCTL_OTERM_FZX_LEFT_MARGIN, -1) - 1;

    line = strtok(s, "\n");

    while (line != NULL)
    {
        line_end = fzx_string_partition_ww(font, line, width);
        *line_end = '\n';
        line = strtok(line_end + 1, "\n");
    }

    return s;
}
