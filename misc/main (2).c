#include <stdio.h>
#include <stdlib.h>

#include "samplerate.h"

int main()
{
	static float input[BUFFER_LEN] = { 0 };
	static float output[BUFFER_LEN];

	SRC_STATE *src_state;
	SRC_DATA src_data;
	int error;
	double max = 0.0;
	sf_count_t output_count = 0;

	sf_seek(infile, 0, SEEK_SET);
	sf_seek(outfile, 0, SEEK_SET);

	/* Initialize the sample rate converter. */
	if ((src_state = src_new(converter, channels, &error)) == NULL)
	{
		printf("\n\nError : src_new() failed : %s.\n\n", src_strerror(error));
		exit(1);
	};

	src_data.end_of_input = 0; /* Set this later. */

	/* Start with zero to force load in while loop. */
	src_data.input_frames = 0;
	src_data.data_in = input;

	src_data.src_ratio = src_ratio;

	src_data.data_out = output;
	src_data.output_frames = BUFFER_LEN / channels;

	while (1)
	{
		/* If the input buffer is empty, refill it. */
		if (src_data.input_frames == 0)
		{
			src_data.input_frames = sf_readf_float(infile, input, BUFFER_LEN / channels);
			src_data.data_in = input;

			/* The last read will not be a full buffer, so snd_of_input. */
			if (src_data.input_frames < BUFFER_LEN / channels)
				src_data.end_of_input = SF_TRUE;
		};

		if ((error = src_process(src_state, &src_data)))
		{
			printf("\nError : %s\n", src_strerror(error));
			exit(1);
		};

		/* Terminate if done. */
		if (src_data.end_of_input && src_data.output_frames_gen == 0)
			break;

		max = apply_gain(src_data.data_out, src_data.output_frames_gen, channels, max, *gain);

		/* Write output. */
		sf_writef_float(outfile, output, src_data.output_frames_gen);
		output_count += src_data.output_frames_gen;

		src_data.data_in += src_data.input_frames_used * channels;
		src_data.input_frames -= src_data.input_frames_used;
	};

	src_delete(src_state);

	if (normalize && max > 1.0)
	{
		*gain = 1.0 / max;
		printf("\nOutput has clipped. Restarting conversion to prevent clipping.\n\n");
		return -1;
	};

    return 0;
}
