#include <stdio.h>
#include <stdlib.h>

#include <ftdi.h>

typedef struct {
    struct ftdi_context * context;
    unsigned char buffer;
} ftdirelayboard_t;

/**
 * @brief frb_open open and init ftdi relay board
 * @param frb
 * @return 0 on sucess, -1 on failure
 */
int frb_open( ftdirelayboard_t * frb )
{
    int err = -1;

    if ( frb )
    {
        // init ftdi device
        frb->context = ftdi_new();
        if ( frb->context )
        {
            if ( ftdi_usb_open( frb->context, 0x0403, 0x6001) == 0)
            {
                ftdi_set_baudrate(frb->context, 921600);
                ftdi_set_bitmode(frb->context,255,4);
                err = 0;
            }
            else
            {
                fprintf( stderr, "unable to open ftdi device: %s", ftdi_get_error_string ( frb->context ));
                ftdi_free(frb->context);
                frb->context = NULL;
            }
        }
        else
            fprintf( stderr, "ftdi_new failed");
        // init i/o buffer
        frb->buffer = 0;
    }

    return err;
}

/**
 * @brief frb_setrelay
 * @param frb
 * @param idx
 * @return
 */
int frb_setrelay( ftdirelayboard_t * frb, unsigned short idx )
{
    unsigned char tmp;
    int err = -1;

    if ( !frb )
        return err;

    if ( idx >= 8 )
    {
        fprintf( stderr, "Bad index");
        return err;
    }

    tmp = frb->buffer | ( 1 << idx );
    err = ftdi_write_data( frb->context,&tmp,1);
    if ( err == 1 )
        frb->buffer = tmp;

    return err;
}

/**
 * @brief frb_unsetrelay
 * @param frb
 * @param idx
 * @return
 */
int frb_unsetrelay( ftdirelayboard_t * frb, unsigned short idx )
{
    unsigned char tmp;
    int err = -1;

    if ( !frb )
        return err;

    if ( idx >= 8 )
    {
        fprintf( stderr, "Bad index");
        return err;
    }

    tmp = frb->buffer & ( 0xff ^ (1 << idx));
    err = ftdi_write_data( frb->context,&tmp,1);
    if ( err == 1 )
        frb->buffer = tmp;

    return err;
}

/**
 * @brief frb_close
 * @param frb
 */
void frb_close( ftdirelayboard_t * frb )
{
    if ( frb && frb->context )
    {
        frb->buffer = 0;
        ftdi_write_data( frb->context,&frb->buffer,1);
        ftdi_free(frb->context);
        frb->context = NULL;
    }
}

/**
 * @brief main
 * @param argc
 * @param argv
 * @return
 */
int main ( int argc, char ** argv )
{
    ftdirelayboard_t frb;

    printf("Start of relayboards project!!!\n");

    if ( frb_open(&frb) == 0 )
    {
        int idx;
        for ( idx = 0; idx < 8; idx++ )
        {
            frb_setrelay( &frb, idx );
            sleep(1);
        }
        for ( idx = 0; idx < 8; idx++ )
        {
            frb_unsetrelay( &frb, idx );
            sleep(1);
        }
        frb_close(&frb);
    }

    return EXIT_SUCCESS;
}
