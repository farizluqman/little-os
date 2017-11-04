char* itoa_f(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i);
    return b;
}

char* itoa(int i){
 return itoa_f(i, "");
}

int atoi(char *str)
{
    int res = 0; // Initialize result

    // Iterate through all characters of input string and
    // update result
    for (int i = 0; str[i] != '\0'; ++i)
        res = res*10 + str[i] - '0';

    // return result.
    return res;
}

// returns the length of a null terminated string
size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len]) {
        ++len;
    }
    return len;
}

void append(char* s, char c)
{
    int len = strlen(s);
    s[len] = c;
    s[len+1] = '\0';
}
