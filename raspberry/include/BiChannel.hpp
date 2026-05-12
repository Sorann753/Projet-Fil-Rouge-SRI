#ifndef BICHANNEL_HPP
#define BICHANNEL_HPP

#include <queue>
#include <mutex>

template <typename T_in, typename T_out>
class BiChannel{
public:
    void pushIn(T_in msg){
        std::lock_guard<std::mutex> lock(inMtx);
        inputBox.push(msg);
    }
    T_in popIn(){
        std::lock_guard<std::mutex> lock(inMtx);
        T_in first = inputBox.front();
        inputBox.pop();

        return first;
    }

    void pushOut(T_out msg){
        std::lock_guard<std::mutex> lock(outMtx);
        outputBox.push(msg);
    }
    T_out popOut(){
        std::lock_guard<std::mutex> lock(outMtx);
        if(outputBox.empty()){
            return T_out(); //default element if empty
        }

        T_out first = outputBox.front();
        outputBox.pop();
        return first;
    }

    bool inputEmpty(){
        return inputBox.empty();
    }

    bool outputEmpty(){
        return outputBox.empty();
    }

    std::size_t outCount(){
        outputBox.size();
    }
    std::size_t inCount(){
        inputBox.size();
    }

private:
    std::queue<T_in> inputBox;
    std::mutex inMtx;

    std::queue<T_out> outputBox;
    std::mutex outMtx;
};

#endif
