/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * qmicli -- Command line interface to control QMI devices
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2012-2015 Aleksander Morgado <aleksander@aleksander.es>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glib.h>
#include <glib/gprintf.h>
#include <gio/gio.h>

#include <libqmi-glib.h>

/* #define QMIPARSE_ONELINE */

int str2qmibin(const char s[], unsigned char a[]);
int str2qmibin(const char s[], unsigned char a[])
{
    int i;

    for (i = 0; s[i * 2] && s[i * 2 + 1]; i++) {
        if (sscanf(&s[i * 2], "%02hhx", &a[i]) == EOF)
            break;
    }

    return i;
}

int getqmibin(FILE *fp, unsigned char *bin);
int getqmibin(FILE *fp, unsigned char *bin)
{
    int len = 0;
    char line[80], *p;

    if (fgets(line, sizeof(line), fp) == NULL)
        return -1;

    for (;;) {
        memset(line, 0x00, sizeof(line));
        if (fgets(line, sizeof(line), fp) == NULL)
            return -1;
        p = line;
        if ((*p == '\n') || (*p == '\0'))
            return len;

        if (len == 0)
            p += 6 + (12 * 3);
        else
            p += 6;

        while (p - line < 6 + (16 * 3)
               && *p != ' '
               && sscanf(p, "%02hhx", bin) != EOF) {
            p += 3;
            bin++;
            len++;
        }
    }
    return len;
}

void dump(const unsigned char p[], int len);
void dump(const unsigned char p[], int len)
{
    int i;

    printf("len = %d, bin = ", len);
    for (i = 0; i < len; i++)
        printf("%02x ", p[i]);
    putchar('\n');
}

int main (int argc, char **argv)
{
    unsigned char qmi[1024];
    int len;
    GByteArray *raw;
    QmiMessage *mes;
#ifndef QMIPARSE_ONELINE
    FILE *fp;
#endif

    if (argc != 2) {
#ifdef QMIPARSE_ONELINE
        fprintf(stderr, "%s <one QMI message text>\n", argv[0]);
#else
        fprintf(stderr, "%s <filename>\n", argv[0]);
#endif
        return (EXIT_FAILURE);
    }

    g_type_init ();

#ifdef QMIPARSE_ONELINE
    len = str2qmibin(argv[1], qmi);
    raw = g_byte_array_new_take(qmi, len);
    mes = qmi_message_new_from_raw(raw, NULL);
    printf("%s", qmi_message_get_printable(mes, ""));
#else
    fp = fopen(argv[1], "r");
    for (;;) {
        len = getqmibin(fp, qmi);
        if (len < 0)
            break;
        dump(qmi, len);
        raw = g_byte_array_new_take(qmi, len);
        mes = qmi_message_new_from_raw(raw, NULL);
        printf("%s", qmi_message_get_printable(mes, ""));
        printf("---------------------------------------------------------------------------\n");
    }
#endif

    return (EXIT_SUCCESS);
}
