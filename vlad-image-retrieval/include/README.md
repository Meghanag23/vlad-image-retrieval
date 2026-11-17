# Classes

Done: `HolidaysDataset`, `FeatureExtractor`, `FeatureCache`. The rest are planned.

| Class | Responsibility |
| --- | --- |
| `HolidaysDataset` | Loads images, query list and ground truth from file names; rotation fix; resizing |
| `FeatureCache` | Saves/loads descriptor matrices as binary files |
| `FeatureExtractor` | SIFT keypoints and descriptors, optional RootSIFT |
| `Vocabulary` | k-means cluster centers (visual words) |
| `VladEncoder` | VLAD vector from an image's descriptors, plus normalizations |
| `BowEncoder` | Bag-of-Words baseline |
| `ImageIndex` | Stores all database vectors and ranks them for a query |
| `Evaluator` | mAP on the INRIA Holidays protocol |
