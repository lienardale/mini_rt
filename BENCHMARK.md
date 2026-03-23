# miniRT Multithreading Benchmark

## System Info
- **CPU cores:** 4
- **Scene:** 25 spheres, 3 lights (programmatic benchmark scene)
- **Resolution:** 1920x1080
- **Runs per config:** 3

## Results

| Threads | Avg Time (ms) | Speedup |
|---------|--------------|---------|
| 1 | 614ms | 1.00x |
| 2 | 305ms | 2.01x |
| 4 | 154ms | 3.97x |

Near-linear scaling is achieved because each pixel is fully independent — no shared mutable state during rendering.

## Threading Approach
- **Library:** POSIX threads (pthreads)
- **Strategy:** Horizontal scanline band partitioning — the image is divided into N horizontal strips, one per thread
- **Thread safety:** Each thread gets its own copy of the camera struct (`cam->pij`/`cam->rij` are modified per-pixel). Scene data (shapes, lights, BVH tree) is read-only. Each thread writes to non-overlapping pixel buffer regions, so no mutex is needed
- **Auto-detection:** Uses `sysconf(_SC_NPROCESSORS_ONLN)` by default; overridable with `--threads N`

## Usage
```bash
./miniRT scene.rt                    # Auto-detect CPU cores (default)
./miniRT scene.rt --threads 4        # Use 4 threads
./miniRT scene.rt --threads 1        # Single-threaded fallback
```

## Running the Benchmark
```bash
make benchmark
```

## GPU Acceleration Consideration
GPU compute (OpenCL/Vulkan) would provide further speedup for ray tracing since each pixel is independent. However, it would require:
- Porting intersection and lighting code to GPU shaders/kernels
- Managing GPU memory transfers for scene data and BVH
- Replacing minilibX framebuffer integration

This is left as a future stretch goal. Pthreads already provides near-linear scaling on multi-core CPUs with minimal code complexity.
