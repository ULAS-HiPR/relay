package i2c

import (
	"errors"
	"fmt"
	"os"
)

type I2C struct {
	bus sbmus
}

type sbmus struct {
	bus *os.File
	addr byte
}

func NewSMbus(bus uint, address byte) (*sbmus){


}

func (smb *SMBus) Bus_open(bus uint) error {

	if smb.bus != nil {
		return errors.New("Can only open one bus at at time")
	}
	path := fmt.Sprintf("/dev/i2c-%d", bus)
	//f, err := os.OpenFile(path, os.O_RDWR, 0600)
	f, err := os.OpenFile(path, os.O_RDWR, 0)
	if err != nil {
		return err
	}
	smb.bus = f
	return nil
}

func (smb *SMBus) Bus_close() error {
	err := smb.bus.Close()
	if err != nil {
		return err
	} else {
		smb.bus = nil
		return nil
	}
}

//should do code to make sure address is what someone says it is, im lazy will do ltr


