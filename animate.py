#!/usr/bin/env python3
"""
Animates the Game of Life for the best individual of each GA generation.
Consecutive generations with the same best individual are shown only once.

Log format: a header line, then one individual per line:
    ROWS COLUMNS GOL_ITERATIONS
    c00 c01 ... c(R-1)(C-1)

For each distinct best individual, the initial grid is shown and then the GOL rules are applied
GOL_ITERATIONS times (override with --steps) before jumping to the next generation's individual.
Rules match life.c: the outer border is kept dead, only inner cells evolve.

Usage:
    python3 animate.py                          # show logbests/banana
    python3 animate.py logbests/banana -o out.mp4   # save video (mp4 needs ffmpeg)
    python3 animate.py -o out.gif --fps 15 --steps 30
"""
import argparse
import sys

import numpy as np
import matplotlib
import matplotlib.pyplot as plt
from matplotlib import animation


def read_log(path):
    """Returns (grids, gol_iterations) from a log file.

    Format: a header line "ROWS COLUMNS GOL_ITERATIONS", then one grid per line
    (ROWS*COLUMNS cell values). Because the C code appends, a file may hold
    several runs (several headers); only the last run is used.
    Old-format files (each line "ROWS COLUMNS cells...", no header) are still
    accepted; gol_iterations is None for them.
    """
    runs = []  # list of [rows, cols, iters, grids]
    with open(path) as f:
        for lineno, line in enumerate(f, 1):
            vals = line.split()
            if not vals:
                continue
            if len(vals) == 3:  # header
                runs.append([int(vals[0]), int(vals[1]), int(vals[2]), []])
                continue
            if runs:
                rows, cols, _, grids = runs[-1]
                cells = vals
            else:  # old format: dimensions at the start of each line
                rows, cols = int(vals[0]), int(vals[1])
                cells = vals[2:]
                if not runs or runs[-1][2] is not None:
                    runs.append([rows, cols, None, []])
                grids = runs[-1][3]
            cells = np.array(cells, dtype=np.uint8)
            if cells.size != rows * cols:
                print(f"warning: line {lineno} has {cells.size} cells, "
                      f"expected {rows * cols}; skipping", file=sys.stderr)
                continue
            grids.append((cells.reshape(rows, cols) & 1).astype(np.uint8))
    if not runs:
        return [], None
    if len(runs) > 1:
        print(f"note: log contains {len(runs)} runs; using the last one",
              file=sys.stderr)
    return runs[-1][3], runs[-1][2]


def step(grid):
    """One GOL generation; border cells stay dead (same as newGen in life.c)."""
    g = grid
    n = (g[:-2, :-2] + g[:-2, 1:-1] + g[:-2, 2:] +
         g[1:-1, :-2] +                g[1:-1, 2:] +
         g[2:, :-2]  + g[2:, 1:-1]  + g[2:, 2:])
    inner = g[1:-1, 1:-1]
    new = np.zeros_like(g)
    new[1:-1, 1:-1] = ((n == 3) | ((inner == 1) & (n == 2))).astype(np.uint8)
    return new


def unique_bests(grids):
    """Collapses consecutive identical individuals.

    Returns a list of (first_gen, last_gen, grid): the generation where this
    individual became the best and the last generation it was still the best.
    """
    runs = []
    for gen, g in enumerate(grids):
        if runs and np.array_equal(runs[-1][2], g):
            runs[-1][1] = gen
        else:
            runs.append([gen, gen, g])
    return [tuple(r) for r in runs]


def frames(runs, steps, hold):
    """Yields (run index, GOL step, grid); the final state is repeated `hold` times."""
    for k, (_, _, grid) in enumerate(runs):
        g = grid
        yield k, 0, g
        for s in range(1, steps + 1):
            g = step(g)
            yield k, s, g
        for _ in range(hold):
            yield k, steps, g


BG = "#0d1117"       # figure background
DEAD = "#161b22"     # dead cells
ALIVE = "#39d353"    # alive cells
TEXT = "#c9d1d9"
ACCENT = "#58a6ff"


def main():
    p = argparse.ArgumentParser(description=__doc__,
                                formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("logfile", nargs="?", default="logbests/banana")
    p.add_argument("-s", "--steps", type=int, default=None,
                   help="GOL iterations per individual "
                        "(default: value from the log header, or 30)")
    p.add_argument("-o", "--output",
                   help="save video to this file (.mp4 via ffmpeg, .gif via pillow)")
    p.add_argument("--fps", type=int, default=15)
    p.add_argument("--pause", type=float, default=2.0,
                   help="seconds to hold the final state of each individual (default 2)")
    p.add_argument("--start", type=int, default=0,
                   help="ignore individuals that became best before this generation")
    p.add_argument("--end", type=int, default=None,
                   help="ignore individuals that became best at/after this generation")
    p.add_argument("--every", type=int, default=1,
                   help="show every Nth distinct best individual")
    p.add_argument("--dpi", type=int, default=100)
    args = p.parse_args()

    if args.output:
        matplotlib.use("Agg")

    grids, header_steps = read_log(args.logfile)
    if args.steps is None:
        args.steps = header_steps if header_steps is not None else 30
    if not grids:
        sys.exit("no individuals found in log")
    n_gens = len(grids)
    runs = unique_bests(grids)
    n_unique = len(runs)
    numbered = [(i + 1, r) for i, r in enumerate(runs)
                if r[0] >= args.start and (args.end is None or r[0] < args.end)]
    numbered = numbered[::args.every]
    if not numbered:
        sys.exit("no individuals in the selected range")
    print(f"{n_gens} generations, {n_unique} distinct best individuals, "
          f"animating {len(numbered)}", file=sys.stderr)
    runs = [r for _, r in numbered]
    hold = round(args.pause * args.fps)
    total_frames = len(runs) * (args.steps + 1 + hold)

    fig = plt.figure(figsize=(7, 7.9), facecolor=BG)
    # grid occupies a square at the bottom, header gets its own strip on top
    ax = fig.add_axes([0.03, 0.05, 0.94, 0.94 * 7 / 7.9])
    ax.set_axis_off()
    cmap = matplotlib.colors.ListedColormap([DEAD, ALIVE])
    img = ax.imshow(runs[0][2], cmap=cmap, vmin=0, vmax=1, interpolation="nearest")
    line1 = fig.text(0.5, 0.965, "", ha="center", va="center",
                     fontsize=13, fontweight="bold", color=ACCENT)
    line2 = fig.text(0.5, 0.93, "", ha="center", va="center",
                     fontsize=10, color=TEXT, family="monospace")

    def update(frame):
        k, s, g = frame
        first, last, _ = runs[k]
        idx = numbered[k][0]
        span = f"gen {first}" if first == last else f"gens {first}\u2013{last}"
        line1.set_text(f"Best of {span}   (individual {idx}/{n_unique})")
        line2.set_text(f"GOL step {s:2d}/{args.steps}   alive: {int(g.sum()):6d}")
        return img, line1, line2

    def update_img(frame):
        img.set_data(frame[2])
        return update(frame)

    anim = animation.FuncAnimation(
        fig, update_img, frames=frames(runs, args.steps, hold), interval=1000 / args.fps,
        blit=False, repeat=False, save_count=total_frames, cache_frame_data=False)

    if args.output:
        if args.output.lower().endswith(".gif"):
            writer = animation.PillowWriter(fps=args.fps)
        else:
            writer = animation.FFMpegWriter(fps=args.fps)

        def progress(i, n):
            print(f"\rrendering frame {i + 1}/{total_frames}", end="", file=sys.stderr)

        anim.save(args.output, writer=writer, dpi=args.dpi,
                  savefig_kwargs={"facecolor": BG}, progress_callback=progress)
        print(f"\nsaved {args.output}", file=sys.stderr)
    else:
        plt.show()


if __name__ == "__main__":
    main()
