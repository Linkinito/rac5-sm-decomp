"""The in-place module image keeps the placement rules of the former place_body."""
import random
from pathlib import Path
import sys
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "tools"))
import level_c_gate as gate

CODE_START = 0x80


def reference_place(rebuilt: bytes, body: bytes, start: int, end: int,
                    claimed: list[tuple[int, int]]) -> bytes:
    """The copying, linear-scan implementation that ModuleImage replaced."""
    for left, right in claimed:
        if start < right and left < end:
            if rebuilt[start:end] != body:
                raise ValueError(f"overlapping level bodies differ at 0x{start:06X}")
            return rebuilt
    claimed.append((start, end))
    if not CODE_START <= start < end <= len(rebuilt) or len(body) != end - start:
        raise ValueError("assembled SNR2 code interval has the wrong extent")
    return rebuilt[:start] + body + rebuilt[end:]


def outcome(run):
    try:
        return "ok", run()
    except ValueError as error:
        return "error", str(error)


def reference(data, claimed, operations):
    def run():
        image, ranges = data, list(claimed)
        for body, start, end in operations:
            image = reference_place(image, body, start, end, ranges)
        return bytes(image)
    return outcome(run)


def current(data, claimed, operations):
    def run():
        image = gate.ModuleImage(data, list(claimed))
        for body, start, end in operations:
            image.place(body, start, end)
        return image.bytes()
    return outcome(run)


class ModuleImageTests(unittest.TestCase):
    def test_places_new_ranges_in_place(self):
        image = gate.ModuleImage(bytes(0x200), [(0x100, 0x108)])
        image.place(b"\x11" * 8, 0x180, 0x188)
        image.place(b"\x22" * 4, 0x188, 0x18C)  # adjacent, not overlapping
        self.assertEqual(image.bytes()[0x180:0x18C], b"\x11" * 8 + b"\x22" * 4)

    def test_overlap_is_accepted_only_when_bytes_agree(self):
        data = bytes(range(256)) * 2
        image = gate.ModuleImage(data, [(0x100, 0x108)])
        image.place(data[0x104:0x10C], 0x104, 0x10C)  # agrees: kept, nothing claimed
        with self.assertRaisesRegex(ValueError, "overlapping level bodies differ at 0x000104"):
            image.place(b"\0" * 8, 0x104, 0x10C)

    def test_extent_rules_of_replace_range(self):
        image = gate.ModuleImage(bytes(0x200), [])
        for body, start, end in ((b"\0" * 8, 0x40, 0x48), (b"\0" * 8, 0x1FC, 0x204),
                                 (b"\0" * 4, 0x100, 0x108), (b"", 0x100, 0x100)):
            with self.assertRaisesRegex(ValueError, "wrong extent"):
                image.place(body, start, end)

    def test_overlapping_initial_claims_are_merged(self):
        image = gate.ModuleImage(bytes(0x200), [(0x100, 0x110), (0x108, 0x120), (0x140, 0x148)])
        self.assertTrue(image.overlaps(0x11C, 0x124))
        self.assertFalse(image.overlaps(0x120, 0x140))
        self.assertTrue(image.overlaps(0x0, 0x200))

    def test_matches_the_reference_on_random_scenarios(self):
        rng = random.Random(20261126)
        for case in range(3000):
            size = rng.choice((0x100, 0x200, 0x400))
            data = rng.randbytes(size)
            claimed = []
            for _ in range(rng.randint(0, 4)):
                start = rng.randrange(0, size)
                claimed.append((start, start + rng.randint(1, 32)))
            operations = []
            for _ in range(rng.randint(1, 12)):
                start = rng.randrange(0x60, size + 8)
                end = start + rng.choice((0, 1, 4, 8, 16, 40))
                mode = rng.random()
                if mode < 0.4:
                    body = data[start:end]
                elif mode < 0.8:
                    body = rng.randbytes(end - start)
                else:
                    body = rng.randbytes(max(0, end - start + rng.choice((-4, 4))))
                operations.append((body, start, end))
            with self.subTest(case=case):
                self.assertEqual(current(data, claimed, operations),
                                 reference(data, claimed, operations))


if __name__ == "__main__":
    unittest.main()
