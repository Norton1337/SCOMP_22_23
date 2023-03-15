/* a) Check which functions are safe to use inside a signal handler. On Linux, consult the signal
safety manual page by typing the command man signal-safety; on other Unix operating systems,
consult the sigaction manual page.


Apenas funçoes assincronas seguras podem ser usadas em funções que lidam com sinais, como por exemplo:

    write
    getpid
    exit
    sigaction
*/

/* b) Why you should not use the printf() function inside a signal handler and, on the other hand, it
is safe use the write() function?

write é mais eficiente que printf, pois não é necessário fazer o buffering do output.
write é assincrono, enquanto que printf é sincrono, e no handler só é recomendado usar funções assincronas.


*/