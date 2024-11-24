package logging

import (
	"io"
	"os"
	"sync"

	"github.com/natefinch/lumberjack"
	"github.com/sirupsen/logrus"

	"github.com/ULAS-HiPR/sat-x/internal/config"
)

var (
	once     sync.Once
	log      *logrus.Logger
	logMutex sync.Mutex
)

func InitLogger(cfg *config.Config) {
	logMutex.Lock()
	defer logMutex.Unlock()

	once.Do(func() {
		log = logrus.New()
		log.SetFormatter(&logrus.TextFormatter{
			FullTimestamp: true,
		})

		level, err := logrus.ParseLevel(cfg.Logs.LogLevel)
		if err != nil {
			log.Warnf("Invalid log level '%s', defaulting to 'info'", cfg.Logs.LogLevel)
			level = logrus.InfoLevel
		}
		log.SetLevel(level)

		// NOTE: Configure lumberjack for log rotation
		lumberjackLogger := &lumberjack.Logger{
			Filename:   cfg.Logs.LogPath,
			MaxSize:    cfg.Logs.MaxSize,
			MaxBackups: cfg.Logs.MaxBackups,
			MaxAge:     cfg.Logs.MaxAge,
			Compress:   cfg.Logs.Compress,
		}

		log.SetOutput(io.MultiWriter(os.Stdout, lumberjackLogger))
	})
}

func GetLogger() *logrus.Logger {
	logMutex.Lock()
	defer logMutex.Unlock()

	if log == nil {
		log.Fatal("Logger is not initialized. Call logging.InitLogger first.")
	}
	return log
}
