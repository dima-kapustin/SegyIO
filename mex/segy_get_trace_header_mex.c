#include <errno.h>
#include <string.h>

#include <segyio/segy.h>
#include "segyutil.h"

#include "matrix.h"
#include "mex.h"

void mexFunction(int nlhs, mxArray *plhs[],
                 int nrhs, const mxArray *prhs[]) {

    char* msg1;
    char* msg2;
    int err;

    FILE* fp = segyfopen( prhs[ 0 ], "r" );
    struct segy_file_format fmt = filefmt( fp );
    int traceno = mxGetScalar( prhs[ 1 ] );

    if( traceno > fmt.traces )
        mexErrMsgIdAndTxt( "segy:get_trace_header:bounds",
                           "Requested trace header does not exist in this file." );

    mwSize dims[ 1 ] = { SEGY_TRACE_HEADER_SIZE };
    plhs[ 0 ] = mxCreateCharArray( 1, dims );
    err = segy_traceheader( fp, traceno, mxGetData( plhs[ 0 ] ), fmt.trace0, fmt.trace_bsize );

    fclose( fp );

    if( err != 0 ) {
        msg1 = "segy:get_trace_header:os";
        msg2 = strerror( errno );
        goto cleanup;
    }

    plhs[ 1 ] = mxCreateDoubleScalar( fmt.traces );
    return;

cleanup:
    mexErrMsgIdAndTxt( msg1, msg2 );
}
