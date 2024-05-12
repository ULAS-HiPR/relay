import numpy as np
from typing import Tuple


def get_ndvi(inp: np.ndarray, re: np.ndarray) -> np.ndarray:
    # NDVI = (NIR - Red) / (NIR + Red)
    denominator = np.where(
        re[:, :, 0] + inp[:, :, 2] != 0, re[:, :, 0] + inp[:, :, 2], 1e-10
    )
    return (re[:, :, 0] - inp[:, :, 2]) / denominator


def get_ndwi(inp: np.ndarray, re: np.ndarray) -> np.ndarray:
    # NDWI = (Green - NIR) / (Green + NIR)
    denominator = np.where(
        inp[:, :, 1] + re[:, :, 2] != 0, inp[:, :, 1] + re[:, :, 2], 1e-10
    )
    return (inp[:, :, 1] - re[:, :, 2]) / denominator


def threshold_ndvi(ndvi: np.ndarray) -> np.ndarray:
    ndvi_regions = np.zeros((256, 256, 3))
    ndvi_regions[ndvi > 0.5, 1] = 1
    return ndvi_regions


def threshold_ndwi(ndwi: np.ndarray) -> np.ndarray:
    ndwi_regions = np.zeros((256, 256, 3))
    ndwi_regions[(ndwi > -0.25) & (ndwi < 0.25), 2] = 1
    return ndwi_regions


def parse_indices(
    inp: np.ndarray, re: np.ndarray, regions: bool = False
) -> (
    Tuple[np.ndarray, np.ndarray]
    or Tuple[np.ndarray, np.ndarray, np.ndarray, np.ndarray]
):
    ndvi = get_ndvi(inp, re)
    ndwi = get_ndwi(inp, re)

    if regions is False:
        return ndvi, ndwi

    ndvi_regions = threshold_ndvi(ndvi)
    ndwi_regions = threshold_ndwi(ndwi)

    return ndvi, ndwi, ndvi_regions, ndwi_regions
