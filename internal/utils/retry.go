package utils

import (
	"time"

	"github.com/avast/retry-go"
)

// Retry retries a function with exponential backoff
func Retry(fn func() error, attempts int, delay time.Duration) error {
	return retry.Do(fn, retry.Attempts(uint(attempts)), retry.Delay(delay))
}
