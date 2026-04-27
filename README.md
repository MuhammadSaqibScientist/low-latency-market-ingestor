# *Low Latency Market Ingestor:* A Deterministic L3-Cache Optimized Market Data Ingestor
A C++20 implementation of a high-frequency market data gateway designed for the Intel Haswell architecture. This project focuses on minimizing tail latency by leveraging CPU core isolation, zero-copy parsing, and lock-free data structures.

## The Architecture (The "Hook")
- **Hardware-Aware Design:** Optimized for the 3MB L3 cache of the i5-4200U to minimize cache misses.
- **Core Affinity:** Utilizing pthread_setaffinity_np to pin the hot-path thread to an isolated logical core.
- **Async I/O:** Powered by Boost.Asio for non-blocking network communication.

## Key Performance Milestones
- **Module 1:** The Ingestor. WebSocket connection with kernel-bypass-lite techniques (SO_REUSEPORT).
- **Module 2:** The Fast-Parser. Hand-written JSON-to-Struct parser (bypassing slow reflection-based libraries).
- **Module 3:** Zero-Latency Logging. A lock-free asynchronous logger to record trades without stalling the main execution loop.

## Hardware/Environment Spec
- **Target CPU:** Intel(R) Core(TM) i5-4200U @ 1.60GHz (Haswell)
- **OS:** Kali Linux / Debian (Kernel 6.x)
- **Optimization Strategy:** CPU Isolation via isolcpus=3, Performance Governor, and AVX2 instruction sets.

## Build Requirements
- **Compiler:** GCC 11+ or Clang 13+ (for C++20 support)
- **Libraries:** Boost.Asio, Boost.Beast
- **Build System:** CMake 3.20+

## My CPU Specs
<details>
<summary> Detailed Hardware Environment (Intel Haswell)</summary>

```
Architecture:                x86_64
  CPU op-mode(s):            32-bit, 64-bit
  Address sizes:             39 bits physical, 48 bits virtual
  Byte Order:                Little Endian
CPU(s):                      4
  On-line CPU(s) list:       0-3
Vendor ID:                   GenuineIntel
  Model name:                Intel(R) Core(TM) i5-4200U CPU @ 1.60GHz
    CPU family:              6
    Model:                   69
    Thread(s) per core:      2
    Core(s) per socket:      2
    Socket(s):               1
    Stepping:                1
    CPU(s) scaling MHz:      49%
    CPU max MHz:             2600.0000
    CPU min MHz:             800.0000
    BogoMIPS:                4589.42
    Flags:                   fpu vme de pse tsc msr pae mce cx8 apic sep mtrr pge mca cmov pat pse36 clflush dts acpi mmx fxsr sse sse2 ss ht tm pbe syscall nx pdpe1gb rdtscp lm constant_tsc arch_perfmon pebs bts rep_good nopl xtopology nonstop_tsc cpuid aperfmperf pni pclmulqdq dtes64 monitor ds_cpl vmx est tm2 ssse3 sdbg fma cx16 xtpr pdcm pcid sse4_1 sse4_2 movbe popcnt tsc_deadline_timer aes xsave avx f16c rdrand lahf_lm abm cpuid_fault epb pti ssbd ibrs ibpb stibp tpr_shadow flexpriority ept vpid ept_ad fsgsbase tsc_adjust bmi1 avx2 smep bmi2 erms invpcid xsaveopt dtherm ida arat pln pts vnmi md_clear flush_l1d
Virtualization features:     
  Virtualization:            VT-x
Caches (sum of all):         
  L1d:                       64 KiB (2 instances)
  L1i:                       64 KiB (2 instances)
  L2:                        512 KiB (2 instances)
  L3:                        3 MiB (1 instance)
NUMA:                        
  NUMA node(s):              1
  NUMA node0 CPU(s):         0-3
Vulnerabilities:             
  Gather data sampling:      Not affected
  Ghostwrite:                Not affected
  Indirect target selection: Not affected
  Itlb multihit:             KVM: Mitigation: VMX disabled
  L1tf:                      Mitigation; PTE Inversion; VMX conditional cache flushes, SMT vulnerable
  Mds:                       Mitigation; Clear CPU buffers; SMT vulnerable
  Meltdown:                  Mitigation; PTI
  Mmio stale data:           Not affected
  Old microcode:             Not affected
  Reg file data sampling:    Not affected
  Retbleed:                  Not affected
  Spec rstack overflow:      Not affected
  Spec store bypass:         Mitigation; Speculative Store Bypass disabled via prctl
  Spectre v1:                Mitigation; usercopy/swapgs barriers and __user pointer sanitization
  Spectre v2:                Mitigation; Retpolines; IBPB conditional; IBRS_FW; STIBP conditional; RSB filling; PBRSB-eIBRS Not affected; BHI Not affected
  Srbds:                     Mitigation; Microcode
  Tsa:                       Not affected
  Tsx async abort:           Not affected
  Vmscape:                   Mitigation; IBPB before exit to userspace
