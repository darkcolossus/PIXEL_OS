include Makefile.inc

SAMPLE_DATA=0001-sampleDataModule.bin

NUM_SONGS=2
SONG_1=A110B110C110D110E110F110G110A210B210C210D210E210F210G210A310B310C310D310E310F310G310A410B410C410D410E410F410G410A510B510C510D510E510F510G510A610B610C610D610E610F610G610
SONG_2=B4200001B4200001B420E460B560A520G420F420C560G420F420E420C560G460F420E420F420D420 

all: sampleCodeModule sampleDataModule

sampleCodeModule:
	cd SampleCodeModule; make

sampleDataModule:
	printf $(NUM_SONGS) >> $(SAMPLE_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SAMPLE_DATA)
	printf $(SONG_1) >> $(SAMPLE_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SAMPLE_DATA)
	printf $(SONG_2) >> $(SAMPLE_DATA) && dd if=/dev/zero bs=1 count=1 >> $(SAMPLE_DATA)

clean:
	cd SampleCodeModule; make clean
	rm -rf *.bin


.PHONY: sampleCodeModule all clean
