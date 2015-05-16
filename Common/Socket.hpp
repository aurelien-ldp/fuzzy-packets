#ifndef SOCKET_HPP_
# define SOCKET_HPP_

class       Socket
{
public:
    enum    e_type {
        UDP,
        TCP
    };
    Socket(e_type type)
    {
        _type = type;
        if (_type == UDP)
        {
            
        }
    }
private:
    e_type  _type;
};

#endif
