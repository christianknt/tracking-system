#ifndef DWM1000_H
#define DWM1000_H


class dwm1000
{
    public:
        dwm1000();
        long leerTag();
        virtual ~dwm1000();
    protected:
    private:
        byte msg1, msg2, msg3, msg4;
        long msg;
};

#endif // DWM1000_H
