use sys_info::{cpu_speed, loadavg, mem_info, proc_total};

#[derive(Debug)]
pub struct SystemInfo {
    pub cpu_speed: u64,
    pub loadavg: sys_info::LoadAvg,
    pub mem_info: sys_info::MemInfo,
    pub proc_total: u64,
}

pub async fn gather_system_info() -> Result<SystemInfo, sys_info::Error> {
    let cpu_speed = cpu_speed()?;
    let loadavg = loadavg()?;
    let mem_info = mem_info()?;
    let proc_total = proc_total()?;

    Ok(SystemInfo {
        cpu_speed,
        loadavg,
        mem_info,
        proc_total,
    })
}

pub fn format_payload(system_info: &SystemInfo, current_time: u64) -> String {
    format!(
        "Time: {}\nCPU Speed: {} MHz\nLoad Average: {:.2}, {:.2}, {:.2}\nMemory Info: Total: {} KB, Free: {} KB, Available: {} KB, Buffers: {} KB, Cached: {} KB\nTotal Processes: {}",
        current_time,
        system_info.cpu_speed,
        system_info.loadavg.one,
        system_info.loadavg.five,
        system_info.loadavg.fifteen,
        system_info.mem_info.total,
        system_info.mem_info.free,
        system_info.mem_info.avail,
        system_info.mem_info.buffers,
        system_info.mem_info.cached,
        system_info.proc_total
    )
}

#[cfg(test)]
mod tests {
    use super::*;

    #[tokio::test]
    async fn test_gather_system_info() {
        let result = gather_system_info().await;

        assert!(result.is_ok());

        let system_info = result.unwrap();

        assert!(system_info.cpu_speed > 0);
        assert!(system_info.loadavg.one >= 0.0);
        assert!(system_info.mem_info.total > 0);
        assert!(system_info.proc_total > 0);
    }

    #[test]
    fn test_format_payload() {
        let system_info = SystemInfo {
            cpu_speed: 2000,
            loadavg: sys_info::LoadAvg {
                one: 1.0,
                five: 2.0,
                fifteen: 3.0,
            },
            mem_info: sys_info::MemInfo {
                total: 4096,
                free: 2048,
                avail: 3072,
                buffers: 128,
                cached: 256,
                swap_total: 0,
                swap_free: 0,
            },
            proc_total: 10,
        };

        let current_time = 123456789;

        let payload = format_payload(&system_info, current_time);

        assert!(payload.contains("Time: 123456789"));
        assert!(payload.contains("CPU Speed: 2000 MHz"));
        assert!(payload.contains("Load Average: 1.00, 2.00, 3.00"));
        assert!(payload.contains("Memory Info: Total: 4096 KB, Free: 2048 KB, Available: 3072 KB, Buffers: 128 KB, Cached: 256 KB"));
        assert!(payload.contains("Total Processes: 10"));
    }
}
