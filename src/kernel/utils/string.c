#include "string.h"

int k_strlen(char *str)
{
	int l = 0;
	while (str[l])
	{
		l++;
	}
	return l;
}

int k_strcmp(char *str1, char *str2)
{
    if (k_strlen(str1) != k_strlen(str2))
    {
        return 0;
    }
    
    int n = k_strlen(str1);

    for (size_t i = 0; i < n; i++)
    {
        if (str1[i] != str2[i])
        {
            return 0;
        }
    }
    
    return 1;
}

char **k_strsplt(char *str, char delim, int *toks_n)
{
    char *s = str;
    int n_toks = 0;

    for (size_t i = 0; i < (k_strlen(s)+1); i++)
    {
        if (s[i] == delim || s[i] == '\0')
        {
            n_toks++;
        }
    }
    *toks_n = n_toks;

    if (n_toks > 1)
    {        
        char **toks = k_malloc(sizeof(char *) * n_toks);

        int x = 0;
        int cur_tok = 0;
        for (size_t i = 0; i < k_strlen(s); i++)
        {
            if (s[i] == delim)
            {
                toks[cur_tok++] = k_malloc(sizeof(char) * (x + 1));
            } else
            {
                x++;
            }
        }
        cur_tok = 0;
        x=0;
        for (size_t i = 0; i < k_strlen(s); i++)
        {
            if (s[i] == delim)
            {
                toks[cur_tok][x] = '\0';
                cur_tok++; x=0;
            } else {
                toks[cur_tok][x++] = s[i];
            }
        }

        return toks;
    } else
    {
        char **toks = (char **)k_malloc(sizeof(char *));
        toks[0] = str;
        toks[1] = NULL;
        return toks;
    }
}

void k_strrev(char *str)
{
    int start = 0;
    int end = k_strlen(str) - 1;
    char temp = ' ';

    while (start < end)
    {
        temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++, end--;
    }
}

char *k_inttostr(int n)
{
    char *buf = k_malloc(sizeof(char) * 12);
    int buf_i = 0;

    int base = 10;
    int rem = 0;

    if (n == 0)
    {
        buf[buf_i++] = '0'; buf[buf_i] = '\0';
        return buf;
    }
    
    while (n > 0)
    {
        rem = n % base;
        buf[buf_i++] = rem + '0';
        n /= base;
    }
    
    if (n < 0)
    {
        buf[buf_i++] = '-';
    }
    buf[buf_i] = '\0';

    k_strrev(buf);

    return buf;
}

int k_strtoint(char *str)
{
    int it = 0;
    int i = 0;
    while (1)
    {
        it += str[i] - '0';
        if (str[i+1] == '\0')
        {
            break;
        }
        it *= 10;
        i++;
    }
    return it;
}

char *k_hextostr(int h)
{
    char *str = k_malloc(sizeof(char) * 24);
    int x = 0;
    str[x++] = '0'; str[x++] = 'x';
    char zeros = 0;

    int tmp;
    int i;
    for (i = 28; i > 0; i -= 4) {
        tmp = (h >> i) & 0xF;
        if (tmp == 0 && zeros == 0) continue;
        zeros = 1;
        if (tmp >= 0xA) {str[x++] = tmp - 0xA + 'a';}
        else {str[x++] = tmp + '0';}
    }

    tmp = h & 0xF;
    if (tmp >= 0xA) {str[x++] = tmp - 0xA + 'a';}
    else {str[x++] = tmp + '0';}

    str[x] = '\0';

    return str;
}