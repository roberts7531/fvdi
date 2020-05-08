/*
 * fVDI Bconout(con) redirection
 *
 * Copyright 1993, Johan Klockars, 2005 Standa Opichal
 * This software is licensed under the GNU General Public License.
 * Please, see LICENSE.TXT for further information.
 */

void bconout_char(long ch)
{
    static short esc = 0;
    static short inverse = 0;
    static short params = 0;
    static char str[] = " ";

    ch &= 0xff;

    if (esc) {
        switch (ch) {
            case 'p':
                inverse = 1;
                break;
            case 'q':
                inverse = 0;
                break;
            case 'K':
                ; /* fallthrough: delete to end of line (I guess) */
            case 'Y':
                params = 2;
                break;

        }

        esc = 0;
        return;
    }

    if (params > 0) {
        params--;
        return;
    }

    switch (ch) {
        default:
            *str = (ch == 32 && inverse) ? '_' : ch;
            puts(str);
    }
}

