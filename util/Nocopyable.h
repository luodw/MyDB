#ifndef _NOCOPYABLE_
#define _NOCOPYABLE_

namespace ionet
{
    /*
    因为这个io库文件很多不能被复制,为了防止复制错误,
    定义了这个类
    */
    class nocopyable
    {
    public:
        nocopyable(){}
        ~nocopyable(){}
    private:
        nocopyable(const nocopyable&);
        nocopyable& operator=(const nocopyable&);
    };
}

#endif
