#ifndef BICHANNEL_HPP
#define BICHANNEL_HPP

template <typename T_in, typename T_out>
class BiChannel{
public:
    void pushIn(T_in msg){

    }
    T_in popIn(){

    }

    void pushOut(T_out){

    }
    T_out popOut(){
        return T_out();
    }

private:
    std::queue<T_in> inputBox;
    std::mutex inMtx;

    std::queue<T_out> outputBox;
    std::mutex outMtx;
};

#endif
