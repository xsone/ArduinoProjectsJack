// Jaspers Thierry Mugler Decoder
// Code ported from http://roel.reijerse.net/thierry/
#ifndef THIERRYDECODER_H_
#define THIERRYDECODER_H_

//Jasper measured micros
//S=3845
//0=2270
//1=950

//Defines are 120% and 80% of it
#define S_MAX 4614
#define S_MIN 3076
#define Z_MAX 2724
#define Z_MIN 1816
#define O_MAX 1140
#define O_MIN 760

enum State
{
Idle, Preamble, Content
};

#endif /* THIERRYDECODER_H_ */
