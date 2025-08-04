# Matrix-Computation-with-SIMD-Optimization-in-C
A high-performance C program that computes the sum of element-wise products between rows of two matrices using **SIMD (Single Instruction, Multiple Data)** optimization via SSE intrinsics. This project demonstrates low-level performance tuning, memory alignment, and precise timing measurement.

🎯 **Goal**:  
Compute `sum[i] = Σ(A[i][j] * B[k][j])` for all `i`, `j`, `k`, where `A` and `B` are 200×198 matrices loaded from a file.

🚀 **Key Features**:
- SIMD acceleration using SSE (`__m128` and `_mm_mul_ps`)
- 16-byte memory alignment for optimal vectorization
- Input/output file handling with error checking
- Fine-grained performance profiling (read, compute, write)
- Data padding to enable vectorized operations

---

## 🧠 Problem Description

Given a `data.txt` file with 400 lines and 198 floating-point numbers per line:
- First 200 lines → matrix `A` (200×198)
- Next 200 lines → matrix `B` (200×198)

For each row `i` in `A`, compute:
$sum[i] = \sum_{k=0}^{199}\sum_{j=0}A[i][j]\times B[k][j]$

That is, each row of `A` is multiplied element-wise with **every row** of `B`, and all results are summed into a single scalar `sum[i]`.

The 200 resulting values are written to `output.txt`.

---

## ⚙️ Optimization Strategy

### 🔹 SIMD Vectorization (SSE)
- Original data size: 198 floats per row → not divisible by 4 (required for `__m128`)
- **Solution**: Pad each row to 200 elements (add 2 zeros) → 200 ÷ 4 = 50 vector groups
- Use `__m128` to process 4 floats at once with `_mm_mul_ps()`
- Reduce each vector result by horizontal sum (manual add of 4 components)

### 🔹 Memory Alignment
- Declare matrices with `__attribute__((aligned(16)))` to ensure 16-byte alignment
- Enables safe casting to `__m128*` and avoids performance penalties

### 🔹 Performance Measurement
- Use `clock_gettime(CLOCK_MONOTONIC)` to measure:
  - File reading time
  - Computation time (core SIMD loop)
  - File writing time

---
## 📊 Performance Results (Measured)

Executed on: Linux (x86_64), GCC 11.4.0, Intel Core i5-XXXX  
Input: `data.txt` with 400×198 floats

| Stage      | Naive Version | SIMD Version | Speedup |
|-----------|---------------|--------------|---------|
| Read      | 6,279,305 ns  | 6,377,885 ns | ~0.99×  |
| Compute   | 16,707,207 ns | 6,846,251 ns | **2.44×** |
| Write     | 49,754 ns     | 52,531 ns    | ~0.95×  |

✅ **Conclusion**: The SIMD-optimized version achieves **2.44× speedup in the computation phase**, demonstrating effective use of SSE vectorization for data-parallel workloads.

## 📋 Code Structure

### `main.c`
- Read `data.txt` into matrices `A` and `B`
- Pad data to 200 columns for SIMD compatibility
- Compute `sum[i]` for each row using `MulSum()` function
- Write results to `output.txt`
- Print timing breakdown

### `MulSum(float A[][200], float B[][200], int i)`
- Takes a row `i` from matrix `A`
- Loops over all rows of `B`
- Uses SSE to compute dot-product-like accumulation
- Returns the total sum of all products

---

## 🛠️ Build & Run

### Prerequisites
- GCC (supports SSE intrinsics)
- POSIX system (Linux/macOS)

### Compile
```
gcc -O2 -msse -o matrix_simd SIMD.c
```
