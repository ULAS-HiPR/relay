package main

import (
	"fmt"
	"log"
	"os"
	"os/exec"
	"time"
)

type Camera struct {
	OutputDir string
	Period    int
	Width     int
	Height    int
}

func (c *Camera) Capture() {
	filename := time.Now().Format(time.RFC3339)
	Filename := fmt.Sprintf("%s/%s.jpg", c.OutputDir, filename)
	cmd := exec.Command("libcamera-jpeg", "-o", Filename, "-t", fmt.Sprintf("%d", c.Period), "--width", fmt.Sprintf("%d", c.Width), "--height", fmt.Sprintf("%d", c.Height))

	err := cmd.Run()
	if err != nil {
		log.Println(err)
	}
}

func (c *Camera) Init() error {
	dir := c.OutputDir

	if err := os.MkdirAll(dir, 0755); err != nil {
		return err
	}

	return nil
}

func main() {
	camera := &Camera{
		OutputDir: "/home/agrisat/relay/data/captures",
		Period:    2000, // keep it small to allow mounting (seems to be ~4 sec at runtime)
		Width:     640,
		Height:    480,
	}

	err := camera.Init()
	if err != nil {
		log.Fatal(err)
	}

	ticker := time.NewTicker(time.Second)
	defer ticker.Stop()

	for {
		<-ticker.C
		camera.Capture()
	}
}
