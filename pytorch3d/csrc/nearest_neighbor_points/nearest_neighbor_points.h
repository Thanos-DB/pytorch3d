// Copyright (c) Facebook, Inc. and its affiliates. All rights reserved.

#pragma once
#include <torch/extension.h>
#include "pytorch3d_cutils.h"

// Compute indices of nearest neighbors in pointcloud p2 to points
// in pointcloud p1.
//
// Args:
//    p1: FloatTensor of shape (N, P1, D) giving a batch of pointclouds each
//        containing P1 points of dimension D.
//    p2: FloatTensor of shape (N, P2, D) giving a batch of pointclouds each
//        containing P2 points of dimension D.
//
// Returns:
//    p1_neighbor_idx: LongTensor of shape (N, P1), where
//                     p1_neighbor_idx[n, i] = j means that the nearest neighbor
//                     to p1[n, i] in the cloud p2[n] is p2[n, j].
//

// Cuda implementation.
at::Tensor nn_points_idx_cuda(at::Tensor p1, at::Tensor p2);

// Implementation which is exposed.
at::Tensor nn_points_idx(at::Tensor p1, at::Tensor p2) {
  if (p1.type().is_cuda() && p2.type().is_cuda()) {
#ifdef WITH_CUDA
    CHECK_CONTIGUOUS_CUDA(p1);
    CHECK_CONTIGUOUS_CUDA(p2);
    return nn_points_idx_cuda(p1, p2);
#else
    AT_ERROR("Not compiled with GPU support.");
#endif
  }
  AT_ERROR("Not implemented on the CPU.");
};
