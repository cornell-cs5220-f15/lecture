.PHONY: submit clean

submit: centroid
	qsub centroid.pbs

centroid: centroid.c centroid.h centroid_timer.c
	icc -o centroid centroid.c centroid_timer.c -qopenmp

clean:
	rm -f centroid
	rm *.o*
