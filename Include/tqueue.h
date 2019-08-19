#ifndef TQUEUE_H

typedef struct {
	int ChannelNum; //# of channel to convert
	uint16_t result; //save the conversion result
	int valid;  //whether this is valid
	int tid;
}	TQueue;

#endif
