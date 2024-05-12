package main

import (
	"fmt"
	"log"
	"os"
	"time"
	"os/exec"
	"sync"
)

type Camera struct {
	OutputDir string
	Width     int
	Height    int
}

func (c *Camera) Capture() error {
	timestamp := time.Now().Format(time.RFC3339)
	filepath := fmt.Sprintf("%s/%s.jpg", c.OutputDir, timestamp)
	cmd := exec.Command("libcamera-jpeg", "-o", filepath, "--width", fmt.Sprintf("%d", c.Width), "--height", fmt.Sprintf("%d", c.Height), "--signal")
	err := cmd.Run()
	if err != nil {
		return err
	}
	return nil
}

func (c *Camera) Init() error {
	if err := os.MkdirAll(c.OutputDir, 0755); err != nil {
		return err
	}
	return nil
}

func main() {
	camera := &Camera{
		OutputDir: "/home/agrisat/relay/camera/results",
		Width:     640,
		Height:    480,
	}

	err := camera.Init()
	if err != nil {
		log.Fatal(err)
	}

	ticker := time.NewTicker(time.Second)
	defer ticker.Stop()

	var wg sync.WaitGroup
	wg.Add(1)
	go func() {
		for {
			<-ticker.C
			camera.Capture()
			fmt.Println("Picture taken")
		}
		wg.Done()
	}()

	wg.Wait()
}
