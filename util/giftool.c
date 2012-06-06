/****************************************************************************

giftool.c - GIF transformation tool.

****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "getopt.h"
#include "getarg.h"
#include "gif_lib.h"

#define PROGRAM_NAME	"giftool"

#define MAX_OPERATIONS	256

struct operation {
    enum {
	background,
	interlace,
	deinterlace,
	delay,
    } mode;    
    union {
	int delaytime;
	int bgcolor;
    };
};
static struct operation operations[MAX_OPERATIONS];
static struct operation *top = operations;

int main(int argc, char **argv)
{
    extern char	*optarg;	/* set by getopt */
    extern int	optind;		/* set by getopt */
    int	i, status;
    GifFileType *GifFileIn, *GifFileOut = (GifFileType *)NULL;
    struct operation *op;

    /*
     * Gather operations from the command line.  We use regular
     * getopt(3) here rather than Gershom's argument getter because
     * preserving the order of operations is important.
     */
    while ((status = getopt(argc, argv, "bd:iI")) != EOF)
    {
	if (top >= operations + MAX_OPERATIONS) {
	    (void)fprintf(stderr, "giftool: too many operations.");
	    exit(EXIT_FAILURE);
	}

	switch (status)
	{
	case 'b':
	    top->mode = background;
	    top->bgcolor = atoi(optarg);

	case 'd':
	    top->mode = delay;
	    top->delaytime = atoi(optarg);
	    break;

	case 'i':
	    top->mode = deinterlace;
	    break;

	case 'I':
	    top->mode = interlace;
	    break;

	default:
	    fprintf(stderr, "usage: giftool [-b bgcolor] [-d delay] [-iI]\n");
	    break;
	}

	++top;
    }	

    /* read in a GIF */
    if ((GifFileIn = DGifOpenFileHandle(0)) == NULL
	|| DGifSlurp(GifFileIn) == GIF_ERROR
	|| ((GifFileOut = EGifOpenFileHandle(1)) == (GifFileType *)NULL))
    {
	PrintGifError();
	exit(EXIT_FAILURE);
    }

    /* perform the operations we've gathered */
    for (op = operations; op < top; op++)
	switch (op->mode)
	{
	case background:
	    GifFileIn->SBackGroundColor = op->bgcolor; 
	    break;

	case interlace:
	    for (i = 0; i < GifFileIn->ImageCount; i++)
		GifFileIn->SavedImages[i].ImageDesc.Interlace = true;
	    break;

	case deinterlace:
	    for (i = 0; i < GifFileIn->ImageCount; i++)
		GifFileIn->SavedImages[i].ImageDesc.Interlace = false;
	    break;

	case delay:
	    for (i = 0; i < GifFileIn->ImageCount; i++)
	    {
		GraphicsControlBlock gcb;

		DGifSavedExtensionToGCB(GifFileIn, i, &gcb);
		gcb.DelayTime = op->delaytime;
		EGifGCBToSavedExtension(&gcb, GifFileIn, i);
	    }
	    break;

	default:
	    (void)fprintf(stderr, "giftool: unknown operation mode\n");
	    exit(EXIT_FAILURE);
	}

    /* write out the results */
    GifFileOut->SWidth = GifFileIn->SWidth;
    GifFileOut->SHeight = GifFileIn->SHeight;
    GifFileOut->SColorResolution = GifFileIn->SColorResolution;
    GifFileOut->SBackGroundColor = GifFileIn->SBackGroundColor;
    GifFileOut->SColorMap = GifMakeMapObject(
				 GifFileIn->SColorMap->ColorCount,
				 GifFileIn->SColorMap->Colors);

    for (i = 0; i < GifFileIn->ImageCount; i++)
	(void) GifMakeSavedImage(GifFileOut, &GifFileIn->SavedImages[i]);

    if (EGifSpew(GifFileOut) == GIF_ERROR)
	PrintGifError();
    else if (DGifCloseFile(GifFileIn) == GIF_ERROR)
	PrintGifError();

    return 0;
}

/* giftool.c ends here */