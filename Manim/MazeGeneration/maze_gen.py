from manim import *

class Cells(Scene):
    def construct(self):
        cell_size = 2

        left_wall = Line(
            UP * cell_size / 2 + LEFT * cell_size / 2,
            DOWN * cell_size / 2 + LEFT * cell_size / 2
        )

        up_wall = Line(
            UP * cell_size / 2 + LEFT * cell_size / 2,
            UP * cell_size / 2 + RIGHT * cell_size / 2
        )

        right_wall = Line(
            UP * cell_size / 2 + RIGHT * cell_size / 2,
            DOWN * cell_size / 2 + RIGHT * cell_size / 2
        )

        bottom_wall = Line(
            DOWN * cell_size / 2 + LEFT * cell_size / 2,
            DOWN * cell_size / 2 + RIGHT * cell_size / 2
        )

        Cell = VGroup(left_wall, up_wall, right_wall, bottom_wall)
        Cell.shift(DOWN * 0.5)

        cell_text = Text(
            "Cell",
            font = "0xProto Nerd Font",
            font_size = 40
        )

        cell_bitmap_text = Text(
            "Cell bitmap",
            font = "0xProto Nerd Font",
            font_size = 32
        )

        binary = "1111"
        bits = VGroup(*[
            Text(
                bit, 
                font="0xProto Nerd Font",
                font_size = 40,
                color = GREEN
            )
            for bit in binary
        ])
        bits.arrange(RIGHT, buff = 0.1)
        bits.shift(RIGHT * 2.5 + DOWN * 0.5)
        cell_bitmap_text.next_to(bits, UP, buff = 0.5)

        binary = "0000"
        new_bits = VGroup(*[
            Text(
                bit, 
                font="0xProto Nerd Font",
                font_size = 40,
                color = DARK_GRAY
            )
            for bit in binary
        ])
        new_bits.arrange(RIGHT, buff = 0.1)
        new_bits.shift(RIGHT * 2.5 + DOWN * 0.5)

        cell_text.next_to(Cell, UP, buff = 0.8)

        self.play(Write(cell_text))
        self.play(Create(left_wall), Create(up_wall))
        self.play(Create(right_wall), Create(bottom_wall))

        self.wait(0.5)

        self.play(Cell.animate.shift(LEFT * 2.5), cell_text.animate.shift(LEFT * 2.5))

        self.wait(0.5)

        self.play(Write(bits))
        self.play(Write(cell_bitmap_text))

        self.wait(0.5)

        walls = [up_wall, right_wall, bottom_wall, left_wall]

        for i in range(4):
            self.play(Transform(bits[3 - i], new_bits[3 - i]), walls[i].animate.set_color(DARK_GRAY))
            self.wait(0.5)
        
        new_bitmap = bits.copy()
        new_bitmap.next_to(bits, DOWN, buff = 0.3)

        new_Cell = Cell.copy()
        new_Cell.shift(RIGHT)
        self.play(Cell.animate.shift(LEFT))
        self.play(Write(new_Cell), Write(new_bitmap))

        above_binary = "1101"
        above_bits = VGroup(*[
            Text(
                bit, 
                font="0xProto Nerd Font",
                font_size = 40,
                color = GREEN if bit == "1" else DARK_GRAY
            )
            for bit in above_binary
        ])
        above_bits.arrange(RIGHT, buff = 0.1)
        above_bits.shift(RIGHT * 2.5 + DOWN * 0.5)

        below_binary = "0111"
        below_bits = VGroup(*[
            Text(
                bit, 
                font="0xProto Nerd Font",
                font_size = 40,
                color = BLUE if bit == "1" else DARK_GRAY
            )
            for bit in below_binary
        ])
        below_bits.arrange(RIGHT, buff = 0.1)
        below_bits.next_to(above_bits, DOWN, buff = 0.3)
        
        self.play(Transform(bits, above_bits), Transform(new_bitmap, below_bits))

        self.play(
            up_wall.animate.set_color(GREEN), left_wall.animate.set_color(GREEN),
            bottom_wall.animate.set_color(GREEN), new_Cell[1].animate.set_color(BLUE),
            new_Cell[2].animate.set_color(BLUE), new_Cell[3].animate.set_color(BLUE)
        )

        self.wait(0.5)

        self.play(
            AnimationGroup(
                AnimationGroup(
                    Unwrite(cell_text), Unwrite(cell_bitmap_text), 
                ),
                AnimationGroup(
                    Uncreate(bits), Uncreate(new_bitmap),
                    Uncreate(Cell), Uncreate(new_Cell)
                ),
                lag_ratio = 0.5
            )
        )

        self.wait(0.5)