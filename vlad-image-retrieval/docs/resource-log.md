# Resource Log

This file lists every resource I used for this project, including all prompts I gave to AI tools and the responses I got back. I update it whenever I use a new resource.

**Project:** Revisiting VLAD: Compact Image Representations for Image Retrieval in OpenCV C++
**Author:** Meghana Reddy Gavinolla

---

## 1. Papers and references

| Resource | How I used it |
| --- | --- |
| H. Jégou, M. Douze, C. Schmid, P. Pérez, "Aggregating local descriptors into a compact image representation," CVPR 2010 | Main paper I am implementing |
| H. Jégou et al., "Aggregating local image descriptors into compact codes," IEEE TPAMI 2012 | Source of power normalization |
| R. Arandjelović, A. Zisserman, "Three things everyone should know to improve object retrieval," CVPR 2012 | Source of RootSIFT |
| R. Arandjelović, A. Zisserman, "All about VLAD," CVPR 2013 | Source of intra-normalization (stretch goal) |
| H. Jégou, M. Douze, C. Schmid, "Hamming embedding and weak geometric consistency for large scale image search," ECCV 2008 | Introduces the INRIA Holidays dataset |

## 2. Data

| Resource | How I used it |
| --- | --- |
| INRIA Holidays dataset (images + ground truth) | Evaluation dataset |

## 3. Libraries

| Resource | How I used it |
| --- | --- |
| OpenCV + opencv_contrib (C++) | SIFT, k-means, Bag-of-Words baseline, PCA, file storage |

## 4. AI tool usage

**Tool:** Claude (Anthropic), claude.ai
**Full conversation:** <!-- Paste the shared chat link here (Share button in claude.ai) -->

### Session 1: Nov 8–9, 2026 — Choosing a paper and writing the proposal

**Summary:** I used Claude to help me choose a paper from the course list, understand the VLAD method, decide what to add beyond the paper, and draft my project proposal. I reviewed the content, checked it against the course guidelines, and edited it before submitting.

| # | My prompt (verbatim) | Summary of the response |
| --- | --- | --- |
| 1 | *(attached the course paper list)* "Here are the Computer vision papers. Where i need to work on the papers and create a project along with I need to add some additional things what paper is missing. Give me the information according to that. First help me to choose the paper. I also attached the papers list." | Compared the papers by difficulty and how much could be added beyond existing OpenCV code. Shortlisted VLAD, Noisy One-Point Homographies, Dense Match Summarization, and GaLAM. Suggested avoiding papers already implemented in OpenCV (GMS, LOGOS, SLIC). |
| 2 | "I wanna do a solo project. Give me the paper that I can work and work on the project for 10 days to get the result." | Recommended VLAD (Jégou et al., CVPR 2010) as the most realistic solo project. Gave a 10-day plan using the INRIA Holidays dataset and mAP, and listed possible extensions (RootSIFT, power normalization, intra-normalization, PCA whitening). |
| 3 | "Give me more overview about this paper and project in more detailed." | Explained the paper's problem (large-scale image search), how VLAD works, the compression part (PCA + product quantization), the datasets, and a detailed project structure with experiments. |
| 4 | "Can you explain me about this paper and project in simple words in detailed" | Re-explained the paper with simple analogies (image fingerprints, Bag-of-Words as word counting, residuals as "difference from average") and a small numeric example. |
| 5 | "What I am gonna add to this project or paper" | Listed my possible contributions: RootSIFT, power normalization, intra-normalization, PCA whitening, an ablation study, a size/speed study, failure analysis, and optionally deep features. |
| 6 | "Do I need to implement all or Can I go with one or two" | Said one or two additions are enough if measured properly. Recommended RootSIFT + power normalization as the core, with the others as optional. |
| 7 | "can you explain what is RootSIFT + power normalization and intra normalization" | Explained each technique: the problem it fixes, how it works, a small numeric example, and short code sketches. |
| 8 | "Now Explain me detailed about this project in simple words and what I'm gonna do further" | Summarized the full pipeline, what is from the paper vs. my additions, the deliverables, and a step-by-step plan. |
| 9 | *(attached the course project guidelines)* "This is the project context. Now, I have do project proposal. Follow the guidelines of the proposal and Give me the information about the project proposal" | Noted that the course requires C++ with OpenCV and adjusted the plan to the course due dates (Nov 9 – Dec 16). Drafted a proposal with all required sections. |
| 10 | "Okay Now I want prepare the project proposal as a student tone humanizer in detailed. Give me the information accordingly" | Rewrote the proposal in a first-person student voice with more detail. |
| 11 | "Give me the proposal document as a student tone humanizer in detailed" | Exported the proposal as a Word file. |
| 12 | "Professor said that if we use Ai we need to add the prompts or something" | Created this resource log. |
| 13 | *(pasted the "Code and data I will use" section)* "Give me in short" | Shortened that proposal section. |
| 14 | *(pasted the bibliography)* "Give me the bibliography as IEEE format" | Reformatted the references in IEEE style with standard venue abbreviations. |
| 15 | "I want to create a GitHub repository now" | Gave steps to create the repository and generated starter files (README, .gitignore, CMakeLists.txt, main.cpp skeleton, folder structure). |
| 16 | "Give me the steps so I will create the repository" | Gave step-by-step instructions for creating the repo on GitHub and uploading the starter files. |
| 17 | "https://github.com/Meghanag23/vlad-image-retrieval.git is it is okay" | Checked the repo: it existed but was still empty. Suggested uploading the files and using the link without `.git`. |

**What I checked or changed myself:**
<!-- Fill in, e.g.: verified the bibliography against the papers, added my GitHub link, chose my preferred presentation dates, trimmed the proposal to 2 pages -->

---

### Session 2: Nov 16, 2026 — Week 1 code: dataset loader, SIFT/RootSIFT, feature cache

**Summary:** I used Claude to write the first part of the code: the Holidays dataset loader, the SIFT/RootSIFT feature extractor, a binary feature cache, and a self-test. Claude compiled and tested the code against OpenCV 4.6 on synthetic images.

| # | My prompt (verbatim) | Summary of the response |
| --- | --- | --- |
| 1 | "Progress reports Starting Nov. 16, there will be (short) weekly individual progress reports submitted to Canvas. Today I want to make some progress of my project" | Wrote `HolidaysDataset`, `FeatureExtractor` (with RootSIFT), `FeatureCache`, and `extract`/`selftest` modes in `main.cpp`. Built with CMake and tested: the self-test passes (RootSIFT rows have unit L2 norm; the cache round trip is exact). Drafted this week's progress report. |

**What I checked or changed myself:**
<!-- e.g. ran it on the real Holidays images, read through each file, adjusted max image size -->

---

### Session template (copy for each new AI session)

### Session N: [date] — [topic]

**Summary:** [what I used AI for and how I used the output]
**Full conversation:** [link]

| # | My prompt (verbatim) | Summary of the response |
| --- | --- | --- |
| 1 | | |

**What I checked or changed myself:**
