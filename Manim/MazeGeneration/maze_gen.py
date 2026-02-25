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


class MazeGenVideoText(Scene):
    def construct(self):

        text = Text(
            "Generate Mazes",
            font = "Great Vibes",
            font_size = 140,
            color ="#aa0000",
            stroke_color = "#aa0000"
        )

        self.play(Write(text), run_time = 3)

        self.wait(0.5)


class Maze3Table(Scene):
    def construct(self):
        mid_line = Line(UP * 2.5, DOWN * 3.5)
        mid_line.set_stroke(PURE_GREEN, 5.0)

        vert_lines = VGroup()

        for i in range(5):
            height = (-2 + i) * DOWN + DOWN * 0.5
            vert_line = Line(LEFT * 5.5 + height, RIGHT * 2.5 + height)
            vert_line.set_stroke(PURE_GREEN, 2.0)
            vert_lines.add(vert_line)
        
        vert_lines[0].set_stroke(PURE_GREEN, 5.0)

        maze_title = Text(
            "Maze 3",
            font = "0xProto Nerd Font",
            font_size = 60,
            weight = BOLD,
            stroke_color = PURE_GREEN,
            color = PURE_GREEN
        )
        maze_title.next_to(mid_line, UP, buff = 0.4)
        maze_title.set_x(vert_lines.get_center()[0])

        self.play(
            Write(maze_title),
            Write(mid_line), 
            
            LaggedStart(
                *[Write(line) for line in vert_lines],
                lag_ratio = 0.15
            )
        )

        self.wait(0.5)

        time_text = Text(
            "Time",
            font = "0xProto Nerd Font",
            font_size = 48,
            stroke_color = PURE_GREEN,
            color = PURE_GREEN
        )
        time_text.next_to(vert_lines[0], UP, buff = 0.1)
        time_text.align_to(vert_lines[0], RIGHT)

        algorithm_text = Text(
            "Algorithm",
            font = "0xProto Nerd Font",
            font_size = 48,
            stroke_color = PURE_GREEN,
            color = PURE_GREEN
        )
        algorithm_text.align_to(time_text, UP)
        algorithm_text.align_to(vert_lines[0], LEFT)

        self.play(Write(algorithm_text), Write(time_text))

        self.wait(0.5)

        algorithms = [
            "Random walk", "DFS",
            "Astar", "BFS",
            "Wall follower"
        ]

        times = [
            "-", "17,2s", "6,2s",
            "5,1s", "1,7s"
        ]

        for i in range(len(algorithms)):
            times_text = Text(
                times[i],
                font = "0xProto Nerd Font",
                font_size = 48,
                stroke_color = PURE_GREEN,
                color = PURE_GREEN
            )
            times_text.move_to(time_text)
            times_text.align_to(time_text, RIGHT)
            times_text.shift(DOWN * (i + 1))

            alg_text = Text(
                algorithms[i],
                font = "0xProto Nerd Font",
                font_size = 48,
                stroke_color = PURE_GREEN,
                color = PURE_GREEN
            )
            alg_text.align_to(times_text, UP)
            alg_text.align_to(algorithm_text, LEFT)

            if i == 0:
                alg_text.shift(UP * 0.25)

            self.play(Write(times_text), Write(alg_text))
            self.wait(0.5)

        self.wait(0.5)


class MazeGenVideoTitle(Scene):
    def construct(self):

        self.camera.background_color = "#040805" 

        step_text = Text(
            "Step 5",
            font = "0xProto Nerd Font",
            font_size = 72,
            color = "#00ff66",
            stroke_color = "#00ff66"
        )
        step_text.to_edge(UP)

        cursor = Rectangle(
            color = "#00ff66",
            fill_color = "#00ff66",
            fill_opacity = 1.0,
            height = 1.1,
            width = 0.5,
        ).move_to(step_text[0])

        self.play(TypeWithCursor(step_text, cursor))
        self.play(Blink(cursor, blinks = 2))

        cursor.move_to(step_text)
        cursor.shift(DOWN * 1.5)
        self.wait(0.25)

        biggest_text = Text(
            "Biggest Maze in",
            font = "IBM Plex Mono",
            font_size = 90,
            color = "#00ff66",
            stroke_color = "#00ff66"
        )
        biggest_text.move_to(step_text)
        biggest_text.shift(DOWN * 2.0)

        world_text = Text(
            "the World",
            font = "IBM Plex Mono",
            font_size = 90,
            color = "#00ff66",
            stroke_color = "#00ff66"
        )
        world_text.move_to(biggest_text)
        world_text.shift(DOWN * 1.5)

        cursor.shift(DOWN * 1.4)
        self.wait(0.5)
        cursor.move_to(biggest_text[0])

        self.play(TypeWithCursor(biggest_text, cursor))

        cursor.move_to(world_text[0])
        self.play(TypeWithCursor(world_text, cursor))
        self.play(Blink(cursor, blinks = 4))