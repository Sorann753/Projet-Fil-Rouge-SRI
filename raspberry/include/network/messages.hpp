#ifndef MESSAGE_HPP
#define MESSAGE_HPP

enum MessageKind{
    // order messages
    Forward = 'F',
    Backward = 'B',
    Turn = 'T',
    StopMoving = 'S',

    // connection message
    Disconnect = 'D',
    Shutdown = 'X',

    // data request
    GetMode = 'G',

    // Config
    SetMode = 'M',
};

#endif // MESSAGE_HPP
