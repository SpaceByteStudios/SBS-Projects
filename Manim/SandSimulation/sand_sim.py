from manim import *
import random

class SandSim(Scene):
    def construct(self):
        self.camera.background_color = "#14141B"

        ruleset = Text(
            "Sand Ruleset",
            font = "0xProto Nerd Font",
            font_size = 36,
            color = WHITE
        )

        rule1 = Text(
            "1. Check below",
            font = "0xProto Nerd Font",
            font_size = 24,
            color = WHITE
        )

        rule2 = Text(
            "2. If blocked -> check diagonals",
            font = "0xProto Nerd Font",
            font_size = 24,
            color = WHITE
        )

        rule3 = Text(
            "3. Else -> hold position",
            font = "0xProto Nerd Font",
            font_size = 24,
            color = WHITE
        )

        size = 3          # total width/height
        step = size / 3   # cell size

        half = size / 2

        v_left  = Line(UP*half + LEFT*half, DOWN*half + LEFT*half)
        v_mid_l = Line(UP*half + LEFT*(half - step), DOWN*half + LEFT*(half - step))
        v_mid_r = Line(UP*half + RIGHT*(half - step), DOWN*half + RIGHT*(half - step))
        v_right = Line(UP*half + RIGHT*half, DOWN*half + RIGHT*half)

        h_top    = Line(LEFT*half + UP*half, RIGHT*half + UP*half)
        h_mid_t  = Line(LEFT*half + UP*(half - step), RIGHT*half + UP*(half - step))
        h_mid_b  = Line(LEFT*half + DOWN*(half - step), RIGHT*half + DOWN*(half - step))
        h_bottom = Line(LEFT*half + DOWN*half, RIGHT*half + DOWN*half)

        grid = VGroup(
            v_left, v_mid_l, v_mid_r, v_right,
            h_top, h_mid_t, h_mid_b, h_bottom
        )

        self.play(Create(v_left), Create(h_top))
        self.play(Create(v_right), Create(h_bottom))

        self.play(
            AnimationGroup(
                AnimationGroup(
                    Create(v_mid_l),
                    Create(h_mid_t),
                    lag_ratio=0
                ),
                AnimationGroup(
                    Create(v_mid_r),
                    Create(h_mid_b),
                    lag_ratio=0
                ),
                lag_ratio=0.4
            )
        )

        self.wait(0.5)
        self.play(grid.animate.shift(LEFT  * 3.5).scale(1.5))

        sand_cell = Square(1.5, fill_opacity = 1.0, fill_color = YELLOW_D, stroke_width = 0.0)
        sand_cell.move_to(LEFT * 3.5)

        self.play(FadeIn(sand_cell))

        ruleset.shift(RIGHT * 3.3 + DOWN * 0.25)

        rule2.next_to(ruleset, DOWN)
        
        rule1.next_to(ruleset, DOWN, buff = 0.7)
        rule1.align_to(rule2, LEFT)

        rule2.next_to(rule1, DOWN, buff = 0.4)
        rule2.align_to(rule1, LEFT)

        rule3.next_to(rule2, DOWN, buff = 0.4)
        rule3.align_to(rule1, LEFT)

        rules_group = VGroup(ruleset, rule1, rule2, rule3)

        rules_rect = SurroundingRectangle(rules_group, buff = 0.3, corner_radius = 0.25)
        rules_rect.stroke_width = 5.0
        rules_rect.set_fill(color = "#222222", opacity = 1.0)
        rules_rect.stroke_color = "#363636"

        full_rules = VGroup(rules_group, rules_rect)
        full_rules.align_to(grid, UP)

        self.play(FadeIn(rules_rect))

        self.wait(0.5)
        self.play(Write(ruleset))

        self.wait(0.5)

        self.wait(0.5)

        down_arrow = Arrow(start = sand_cell.get_center(), end = sand_cell.get_center() + DOWN * step * 1.7, buff = 0.2)
        down_arrow.stroke_width = 16
        down_arrow.stroke_color = RED
        down_arrow.fill_color = RED

        self.wait(0.5)
        self.play(Write(rule1))

        self.play(FadeIn(down_arrow))

        self.wait(0.5)
        sand_cell.shift(DOWN * step * 1.5)

        self.wait(1.0)
        sand_cell.shift(UP * step * 1.5)

        blocked_down_cell = Square(1.5, fill_opacity = 1.0, fill_color = GREY_D, stroke_width = 0.0)
        blocked_down_cell.move_to(LEFT * 3.5 + DOWN * step * 1.5)

        self.play(FadeIn(blocked_down_cell))

        self.wait(0.5)

        down_left_arrow = Arrow(start = sand_cell.get_center(), end = sand_cell.get_center() + DOWN * step * 1.5 + LEFT * step * 1.5, buff = 0.2)
        down_left_arrow.stroke_width = 16
        down_left_arrow.stroke_color = RED
        down_left_arrow.fill_color = RED

        down_right_arrow = Arrow(start = sand_cell.get_center(), end = sand_cell.get_center() + DOWN * step * 1.5 + RIGHT * step * 1.5, buff = 0.2)
        down_right_arrow.stroke_width = 16
        down_right_arrow.stroke_color = RED
        down_right_arrow.fill_color = RED

        self.wait(0.5)
        self.play(Write(rule2))

        self.play(FadeIn(down_left_arrow))

        self.wait(0.5)
        sand_cell.shift(DOWN * step * 1.5 + LEFT * step * 1.5)

        self.wait(1.0)
        sand_cell.shift(UP * step * 1.5 + RIGHT * step * 1.5)

        blocked_down_left_cell = Square(1.5, fill_opacity = 1.0, fill_color = GREY_D, stroke_width = 0.0)
        blocked_down_left_cell.move_to(LEFT * 3.5 + DOWN * step * 1.5 + LEFT * step * 1.5)

        self.play(FadeIn(blocked_down_left_cell))

        self.wait(0.5)

        self.play(FadeIn(down_right_arrow))

        self.wait(0.5)
        sand_cell.shift(DOWN * step * 1.5 + RIGHT * step * 1.5)

        self.wait(1.0)
        sand_cell.shift(UP * step * 1.5 + LEFT * step * 1.5)

        blocked_down_right_cell = Square(1.5, fill_opacity = 1.0, fill_color = GREY_D, stroke_width = 0.0)
        blocked_down_right_cell.move_to(LEFT * 3.5 + DOWN * step * 1.5 + RIGHT * step * 1.5)

        self.play(FadeIn(blocked_down_right_cell))

        self.wait(0.5)

        self.play(Write(rule3))

        self.wait(0.5)











class SandSimAction(Scene):
    def construct(self):
        self.camera.background_color = "#14141B"

        ruleset = Text(
            "Sand Ruleset",
            font = "0xProto Nerd Font",
            font_size = 36,
            color = WHITE
        )

        rule1 = Text(
            "1. Check below",
            font = "0xProto Nerd Font",
            font_size = 24,
            color = WHITE
        )

        rule2 = Text(
            "2. If blocked -> check diagonals",
            font = "0xProto Nerd Font",
            font_size = 24,
            color = WHITE
        )

        rule3 = Text(
            "3. Else -> hold position",
            font = "0xProto Nerd Font",
            font_size = 24,
            color = WHITE
        )

        size = 3          # total width/height
        step = size / 3   # cell size

        half = size / 2

        v_left  = Line(UP*half + LEFT*half, DOWN*half + LEFT*half)
        v_mid_l = Line(UP*half + LEFT*(half - step), DOWN*half + LEFT*(half - step))
        v_mid_r = Line(UP*half + RIGHT*(half - step), DOWN*half + RIGHT*(half - step))
        v_right = Line(UP*half + RIGHT*half, DOWN*half + RIGHT*half)

        h_top    = Line(LEFT*half + UP*half, RIGHT*half + UP*half)
        h_mid_t  = Line(LEFT*half + UP*(half - step), RIGHT*half + UP*(half - step))
        h_mid_b  = Line(LEFT*half + DOWN*(half - step), RIGHT*half + DOWN*(half - step))
        h_bottom = Line(LEFT*half + DOWN*half, RIGHT*half + DOWN*half)

        grid = VGroup(
            v_left, v_mid_l, v_mid_r, v_right,
            h_top, h_mid_t, h_mid_b, h_bottom
        )

        for part in grid:
            part.z_index = 1000

        self.add(v_left, h_top)
        self.add(v_right, h_bottom)
        self.add(v_mid_l, h_mid_t)
        self.add(v_mid_r, h_mid_b)

        grid.shift(LEFT  * 3).scale(1.5)

        sand_cell = Square(1.5, fill_opacity = 1.0, fill_color = YELLOW_D, stroke_width = 0.0)
        sand_cell.move_to(LEFT * 3)
        sand_cell.z_index = 1

        self.add(sand_cell)

        ruleset.shift(RIGHT * 3.5 + DOWN * 0.25)

        rule2.next_to(ruleset, DOWN)
        
        rule1.next_to(ruleset, DOWN, buff = 0.7)
        rule1.align_to(rule2, LEFT)

        rule2.next_to(rule1, DOWN, buff = 0.4)
        rule2.align_to(rule1, LEFT)

        rule3.next_to(rule2, DOWN, buff = 0.4)
        rule3.align_to(rule1, LEFT)

        rules_group = VGroup(ruleset, rule1, rule2, rule3)

        rules_rect = SurroundingRectangle(rules_group, buff = 0.3, corner_radius = 0.25)
        rules_rect.stroke_width = 5.0
        rules_rect.set_fill(color = "#222222", opacity = 1.0)
        rules_rect.stroke_color = "#363636"

        full_rules = VGroup(rules_group, rules_rect)
        full_rules.align_to(grid, UP).shift(UP * 0.05)

        self.add(rules_rect)
        self.add(ruleset)
        self.add(rule1)
        self.add(rule2)
        self.add(rule3)

        # circle = Circle(0.1).set_fill(RED, 1.0)
        # self.add(circle)
        # self.wait(2.0)

        down_arrow = Arrow(start = sand_cell.get_center(), end = sand_cell.get_center() + DOWN * step * 1.7, buff = 0.2)
        down_arrow.stroke_width = 16
        down_arrow.stroke_color = RED
        down_arrow.fill_color = RED
        down_arrow.z_index = 100000

        self.add(down_arrow)

        sand_cell.shift(DOWN * step * 1.5)

        sand_cell.shift(UP * step * 1.5)

        blocked_down_cell = Square(1.5, fill_opacity = 1.0, fill_color = GREY_D, stroke_width = 0.0)
        blocked_down_cell.move_to(LEFT * 3 + DOWN * step * 1.5)

        self.add(blocked_down_cell)

        down_left_arrow = Arrow(start = sand_cell.get_center(), end = sand_cell.get_center() + DOWN * step * 1.5 + LEFT * step * 1.5, buff = 0.2)
        down_left_arrow.stroke_width = 16
        down_left_arrow.stroke_color = RED
        down_left_arrow.fill_color = RED
        down_left_arrow.z_index = 100000

        down_right_arrow = Arrow(start = sand_cell.get_center(), end = sand_cell.get_center() + DOWN * step * 1.5 + RIGHT * step * 1.5, buff = 0.2)
        down_right_arrow.stroke_width = 16
        down_right_arrow.stroke_color = RED
        down_right_arrow.fill_color = RED
        down_right_arrow.z_index = 100000

        self.add(down_left_arrow)

        sand_cell.shift(DOWN * step * 1.5 + LEFT * step * 1.5)

        sand_cell.shift(UP * step * 1.5 + RIGHT * step * 1.5)

        blocked_down_left_cell = Square(1.5, fill_opacity = 1.0, fill_color = GREY_D, stroke_width = 0.0)
        blocked_down_left_cell.move_to(LEFT * 3 + DOWN * step * 1.5 + LEFT * step * 1.5)

        self.add(blocked_down_left_cell)

        self.add(down_right_arrow)

        sand_cell.shift(DOWN * step * 1.5 + RIGHT * step * 1.5)

        sand_cell.shift(UP * step * 1.5 + LEFT * step * 1.5)

        blocked_down_right_cell = Square(1.5, fill_opacity = 1.0, fill_color = GREY_D, stroke_width = 0.0)
        blocked_down_right_cell.move_to(LEFT * 3 + DOWN * step * 1.5 + RIGHT * step * 1.5)

        self.add(blocked_down_right_cell)

        self.wait(0.5)

        arrows = VGroup(down_arrow, down_left_arrow, down_right_arrow)

        self.play(
            FadeOut(ruleset), FadeOut(rule1), FadeOut(rule2),
            FadeOut(rule3), FadeOut(rules_rect), FadeOut(arrows)
        )

        self.play(
            FadeOut(sand_cell), FadeOut(blocked_down_cell), 
            FadeOut(blocked_down_left_cell), FadeOut(blocked_down_right_cell)
        )

        self.wait(0.5)

        offset = RIGHT * step * 1.5 + UP * step * 1.5

        self.play(grid.animate.move_to(ORIGIN - offset * 0.6 * 1.333).scale(0.6 * 1.333))

        self.wait(0.25)

        new_mid_r = Line(
            v_right.get_end() + RIGHT * 1.2,
            v_right.get_start() + UP * 2.4 + RIGHT * 1.2
        )

        new_right = Line(
            v_right.get_end() + RIGHT * 2.4,
            v_right.get_start() + UP * 2.4 + RIGHT * 2.4
        )

        new_mid_top = Line(
            h_top.get_start() + UP * 1.2,
            h_top.get_end() + RIGHT * 2.4 + UP * 1.2
        )

        new_top = Line(
            h_top.get_start() + UP * 2.4,
            h_top.get_end() + RIGHT * 2.4 + UP * 2.4
        )

        self.play(
            AnimationGroup(
                AnimationGroup(
                    AnimationGroup(
                        v_left.animate.put_start_and_end_on(
                            v_left.get_start() + UP * 2.4,
                            v_left.get_end()
                        ),
                        v_mid_l.animate.put_start_and_end_on(
                            v_mid_l.get_start() + UP * 2.4,
                            v_mid_l.get_end()
                        ),
                        v_mid_r.animate.put_start_and_end_on(
                            v_mid_r.get_start() + UP * 2.4,
                            v_mid_r.get_end()
                        ),
                        v_right.animate.put_start_and_end_on(
                            v_right.get_start() + UP * 2.4,
                            v_right.get_end()
                        ),

                        lag_ratio = 0.25
                    ),

                    AnimationGroup(
                        h_bottom.animate.put_start_and_end_on(
                            h_bottom.get_start(),
                            h_bottom.get_end() + RIGHT * 2.4
                        ),
                        h_mid_b.animate.put_start_and_end_on(
                            h_mid_b.get_start(),
                            h_mid_b.get_end() + RIGHT * 2.4
                        ),
                        h_mid_t.animate.put_start_and_end_on(
                            h_mid_t.get_start(),
                            h_mid_t.get_end() + RIGHT * 2.4
                        ),
                        h_top.animate.put_start_and_end_on(
                            h_top.get_start(),
                            h_top.get_end() + RIGHT * 2.4
                        ),

                        lag_ratio = 0.25
                    )
                ),

                AnimationGroup(
                    AnimationGroup(
                        Create(new_mid_r),
                        Create(new_right),
                        lag_ratio = 0.25
                    ),

                    AnimationGroup(
                        Create(new_mid_top),
                        Create(new_top),
                        lag_ratio = 0.25
                    )
                ),

                lag_ratio = 0.33
            )
        )

        new_grid = VGroup(grid, new_mid_r, new_mid_top, new_right, new_top)

        for part in new_grid:
            part.z_index = 1000

        self.wait(0.5)

        step = 1.2

        sand_cell1 = Square(1.2, fill_opacity = 1.0, fill_color = YELLOW_D, stroke_width = 0.0)
        sand_cell1.shift(UP * step * 2.0)

        self.add(sand_cell1)
        self.wait(0.15)

        for i in range(4):
            sand_cell1.shift(DOWN * step)
            self.wait(0.15)
        
        sand_cell2 = Square(1.2, fill_opacity = 1.0, fill_color = YELLOW_D, stroke_width = 0.0)
        sand_cell2.shift(UP * step * 2.0)

        self.add(sand_cell2)
        self.wait(0.15)

        for i in range(3):
            sand_cell2.shift(DOWN * step)
            self.wait(0.15)
        
        sand_cell2.shift(DOWN * step + LEFT * step)
        self.wait(0.15)

        sand_cell3 = Square(1.2, fill_opacity = 1.0, fill_color = YELLOW_D, stroke_width = 0.0)
        sand_cell3.shift(UP * step * 2.0)

        self.add(sand_cell3)
        self.wait(0.15)

        for i in range(3):
            sand_cell3.shift(DOWN * step)
            self.wait(0.15)
        
        sand_cell3.shift(DOWN * step + RIGHT * step)
        self.wait(0.15)

        sand_cell4 = Square(1.2, fill_opacity = 1.0, fill_color = YELLOW_D, stroke_width = 0.0)
        sand_cell4.shift(UP * step * 2.0)

        self.add(sand_cell4)
        self.wait(0.15)

        for i in range(3):
            sand_cell4.shift(DOWN * step)
            self.wait(0.15)
        
        sand_cell5 = Square(1.2, fill_opacity = 1.0, fill_color = YELLOW_D, stroke_width = 0.0)
        sand_cell5.shift(UP * step * 2.0)

        self.add(sand_cell5)
        self.wait(0.15)

        for i in range(2):
            sand_cell5.shift(DOWN * step)
            self.wait(0.15)
        
        sand_cell5.shift(DOWN * step + LEFT * step)
        self.wait(0.15)
    
        sand_cell5.shift(DOWN * step + LEFT * step)
        self.wait(0.15)
        
        sand_cell6 = Square(1.2, fill_opacity = 1.0, fill_color = YELLOW_D, stroke_width = 0.0)
        sand_cell6.shift(UP * step * 2.0)

        self.add(sand_cell6)
        self.wait(0.15)

        for i in range(2):
            sand_cell6.shift(DOWN * step)
            self.wait(0.15)
        
        sand_cell6.shift(DOWN * step + LEFT * step)
        self.wait(0.15)

        sand_cell7 = Square(1.2, fill_opacity = 1.0, fill_color = YELLOW_D, stroke_width = 0.0)
        sand_cell7.shift(UP * step * 2.0)

        self.add(sand_cell7)
        self.wait(0.15)

        for i in range(2):
            sand_cell7.shift(DOWN * step)
            self.wait(0.15)
        
        sand_cell7.shift(DOWN * step + RIGHT * step)
        self.wait(0.15)

        sand_cell7.shift(DOWN * step + RIGHT * step)
        self.wait(0.15)

        sand_cell8 = Square(1.2, fill_opacity = 1.0, fill_color = YELLOW_D, stroke_width = 0.0)
        sand_cell8.shift(UP * step * 2.0)

        self.add(sand_cell8)
        self.wait(0.15)

        for i in range(2):
            sand_cell8.shift(DOWN * step)
            self.wait(0.15)
        
        sand_cell8.shift(DOWN * step + RIGHT * step)
        self.wait(0.15)

        sand_cell9 = Square(1.2, fill_opacity = 1.0, fill_color = YELLOW_D, stroke_width = 0.0)
        sand_cell9.shift(UP * step * 2.0)

        self.add(sand_cell9)
        self.wait(0.15)

        for i in range(2):
            sand_cell9.shift(DOWN * step)
            self.wait(0.15)
        
        self.wait(0.5)

        self.play(
            AnimationGroup(
                AnimationGroup(
                    sand_cell9.animate.scale(0.0),

                    AnimationGroup(
                        sand_cell6.animate.scale(0.0),
                        sand_cell8.animate.scale(0.0)
                    ),

                    AnimationGroup(
                        sand_cell7.animate.scale(0.0),
                        sand_cell5.animate.scale(0.0)
                    ),

                    lag_ratio = 0.25
                ),

                AnimationGroup(
                    sand_cell4.animate.scale(0.0),

                    AnimationGroup(
                        sand_cell2.animate.scale(0.0),
                        sand_cell3.animate.scale(0.0)
                    ),

                    sand_cell1.animate.scale(0.0),

                    lag_ratio = 0.25
                ),

                lag_ratio = 0.25
            )
            
        )

        self.wait(0.25)

        self.play(
            AnimationGroup(
                AnimationGroup(
                    v_left.animate.put_start_and_end_on(
                        v_left.get_start(),
                        v_left.get_start()
                    ),
                    v_mid_l.animate.put_start_and_end_on(
                        v_mid_l.get_start(),
                        v_mid_l.get_start()
                    ),
                    v_mid_r.animate.put_start_and_end_on(
                        v_mid_r.get_start(),
                        v_mid_r.get_start()
                    ),
                    v_right.animate.put_start_and_end_on(
                        v_right.get_start(),
                        v_right.get_start()
                    ),
                    new_mid_r.animate.put_start_and_end_on(
                        new_mid_r.get_end(),
                        new_mid_r.get_end()
                    ),
                    new_right.animate.put_start_and_end_on(
                        new_right.get_end(),
                        new_right.get_end()
                    ),

                    lag_ratio = 0.25
                ),

                AnimationGroup(
                    h_bottom.animate.put_start_and_end_on(
                        h_bottom.get_end(),
                        h_bottom.get_end()
                    ),
                    h_mid_b.animate.put_start_and_end_on(
                        h_mid_b.get_end(),
                        h_mid_b.get_end()
                    ),
                    h_mid_t.animate.put_start_and_end_on(
                        h_mid_t.get_end(),
                        h_mid_t.get_end()
                    ),
                    h_top.animate.put_start_and_end_on(
                        h_top.get_end(),
                        h_top.get_end()
                    ),
                    new_mid_top.animate.put_start_and_end_on(
                        new_mid_top.get_end(),
                        new_mid_top.get_end()
                    ),
                    new_top.animate.put_start_and_end_on(
                        new_top.get_end(),
                        new_top.get_end()
                    ),

                    lag_ratio = 0.25
                )
            ),
        )

        self.wait(0.25)













class GridSim(Scene):
    def construct(self):
        self.camera.background_color = "#14141B"

        grid = VGroup()

        for y in range(-2, 3):
            for x in range(-2, 3):
                square = Square(1.2)
                square.move_to(DOWN * y * 1.2 + RIGHT * x * 1.2)

                grid.add(square)
        
        self.add(grid)

        self.wait(0.5)

        width = Text(
            "5",
            font = "0xProto Nerd Font",
            font_size = 36,
            color = WHITE
        )

        height = Text(
            "5",
            font = "0xProto Nerd Font",
            font_size = 36,
            color = WHITE
        )

        width.next_to(grid, DOWN, buff = 0.3)
        height.next_to(grid, RIGHT, buff = 0.3)

        self.play(Write(width), Write(height))

        self.wait(0.25)

        self.play(Unwrite(width, run_time = 0.5), Unwrite(height, run_time = 0.5))

        self.wait(0.5)

        red_square = Square(1.2, color = RED)
        red_square.move_to(DOWN * 2 * 1.2 + RIGHT * 2 * 1.2)

        self.play(Create(red_square))
        self.wait(0.25)

        for f in range(3):
            for i in range(5):
                for j in range(4):
                    red_square.shift(LEFT * 1.2)
                    self.wait(0.07)
                
                if(i < 4):
                    red_square.shift(UP * 1.2 + RIGHT * 4 * 1.2)
                    self.wait(0.07)
                else:
                    if f < 2:
                        red_square.shift(DOWN * 4 * 1.2 + RIGHT * 4 * 1.2)
                        self.wait(0.07)
        
        red_square.set_opacity(0.0)
        self.wait(0.5)

        bool_grid = VGroup()

        for cell in grid:
            update_bool = Text(
                "false",
                font = "0xProto Nerd Font",
                font_size = 18,
                color = RED
            )

            update_bool.move_to(cell.get_center())

            bool_grid.add(update_bool)
        
        self.play(
            LaggedStart(
                *[FadeIn(text, run_time = 0.5) for text in bool_grid],
                lag_ratio = 0.1
            )
        )

        self.wait(0.5)

        new_bool_grid = VGroup()

        new_text_bool = Text(
            "true",
            font = "0xProto Nerd Font",
            font_size = 18,
            color = BLUE
        )

        index = len(bool_grid) - 1

        new_text_bool.move_to(bool_grid[index].get_center())
        bool_grid[index].set_opacity(0.0)
        
        new_bool_grid.add(new_text_bool)
        self.add(new_text_bool)

        index -= 1

        red_square.move_to(DOWN * 2 * 1.2 + RIGHT * 2 * 1.2)
        red_square.set_stroke(opacity = 1.0)
        self.wait(0.25)

        for i in range(5):
            for j in range(4):
                red_square.shift(LEFT * 1.2)
                self.wait(0.07)

                new_update_bool = Text(
                    "true",
                    font = "0xProto Nerd Font",
                    font_size = 18,
                    color = BLUE
                )

                new_update_bool.move_to(bool_grid[index].get_center())
                bool_grid[index].set_opacity(0.0)
                
                new_bool_grid.add(new_update_bool)
                self.add(new_update_bool)

                index -= 1
                
            if(i < 4):
                red_square.shift(UP * 1.2 + RIGHT * 4 * 1.2)
                self.wait(0.07)

                new_update_bool = Text(
                    "true",
                    font = "0xProto Nerd Font",
                    font_size = 18,
                    color = BLUE
                )

                new_update_bool.move_to(bool_grid[index].get_center())
                bool_grid[index].set_opacity(0.0)
                
                new_bool_grid.add(new_update_bool)
                self.add(new_update_bool)

                index -= 1
        
        red_square.set_opacity(0.0)
        self.wait(0.5)

        for new_text in new_bool_grid:
            new_text.set_opacity(0.0)
        
        for text in bool_grid:
            text.set_opacity(1.0)
        
        self.wait(0.5)
        
        self.play(
            LaggedStart(
                *[FadeOut(text, run_time = 0.5) for text in reversed(bool_grid)],
                lag_ratio = 0.1
            )
        )
        
        self.wait(0.5)

        red_square.move_to(DOWN * 2 * 1.2 + RIGHT * 2 * 1.2)
        red_square.set_stroke(opacity = 1.0)
        self.wait(0.25)

        for i in range(5):
            for j in range(4):
                red_square.shift(LEFT * 1.2)
                self.wait(0.07)
                
            if(i < 4):
                red_square.shift(UP * 1.2 + RIGHT * 4 * 1.2)
                self.wait(0.07)
        
        red_square.set_opacity(0.0)
        self.wait(0.5)








class DrawCalls(Scene):
    def construct(self):
        self.camera.background_color = "#14141B"

        cells_amount = 25
        grid_size = 4
        
        cells = VGroup()
        cell_size = grid_size / cells_amount
        top_left_pos = UP * 2 + LEFT * 6
        
        for y in range(cells_amount):
            for x in range(cells_amount):
                new_cell = Square(cell_size * 1.2)
                new_cell.set_stroke(width = 0.0)
                new_cell.set_fill(YELLOW_D, 1.0)
                new_cell.move_to(top_left_pos + RIGHT * x * cell_size + DOWN * y * cell_size)
                cells.add(new_cell)

        self.wait(0.5)

        cpu = SVGMobject("cpu-icon.svg").scale(0.75)
        gpu = SVGMobject("gpu-icon.svg").scale(0.75)

        gpu.shift(RIGHT * 5.5)

        cpu.set_color(WHITE)
        gpu.set_color(WHITE)

        self.play(Write(cpu))
        self.play(Write(gpu))
        self.wait(0.5)

        self.play(GrowFromCenter(cells[0]))
        self.wait(0.5)

        arrow = Arrow(
            start = cpu.get_center(),
            end = gpu.get_center(),
            buff = 1.25,
            stroke_width = 14.0
        )
        arrow.set_color(WHITE)
        self.play(Write(arrow))
        self.wait(0.5)

        file = SVGMobject("file-icon.svg").set_color(WHITE).scale(0.65)
        offset = gpu.get_center() - cpu.get_center()

        for i in range(1, 3):
            file.next_to(cpu, UP)
            self.play(FadeIn(file, run_time = 0.5))
            self.play(file.animate.shift(offset), run_time = 0.5)
            self.play(FadeOut(file, run_time = 0.5))
            self.wait(0.5)

            self.play(GrowFromCenter(cells[i], run_time = 0.5))
            self.wait(0.5)
        
        file.next_to(cpu, UP)
        self.play(FadeIn(file, run_time = 0.5))
        self.play(file.animate.shift(offset), run_time = 0.5)
        self.play(FadeOut(file, run_time = 0.5))
        self.wait(0.5)

        files = VGroup()
        files_amount = 50

        for f in range(files_amount):
            new_file = SVGMobject("file-icon.svg").set_color(WHITE).scale(0.3)
            new_file.next_to(cpu, UP, buff = 0.75)
            new_file.shift(UP * random.uniform(-0.75, 0.75) + RIGHT * random.uniform(-0.5, 0.5))

            files.add(new_file)

        self.play(
            AnimationGroup(
                LaggedStart(
                    *[
                        Succession(
                            FadeIn(f, run_time = 0.2),
                            f.animate.shift(offset),
                            FadeOut(f, run_time = 0.2)
                        )
                        for f in files
                    ],
                    lag_ratio = 0.05
                ),

                LaggedStart(
                    *[GrowFromCenter(cell, run_time = 0.5) for cell in cells[3:]],
                    lag_ratio=0.01
                )
            )
        )

        self.wait(1.0)

        self.play(
            LaggedStart(
                *[ShrinkToCenter(cell, run_time = 0.5) for cell in cells],
                lag_ratio=0.005
            )
        )

        self.wait(0.5)

        triangle_cells = VGroup()
        h_cell_size = cell_size / 2.0

        for cell in cells:
            triangle_cell = VGroup()

            p_1 = cell.get_center() + UP * h_cell_size + LEFT * h_cell_size
            p_2 = cell.get_center() + UP * h_cell_size + RIGHT * h_cell_size
            p_3 = cell.get_center() + DOWN * h_cell_size + LEFT * h_cell_size
            p_4 = cell.get_center() + DOWN * h_cell_size + RIGHT * h_cell_size

            up_triangle = Polygon(p_1, p_2, p_3)
            down_triangle = Polygon(p_4, p_3, p_2)

            up_triangle.set_stroke(WHITE, 0.75)
            down_triangle.set_stroke(WHITE, 0.75)

            triangle_cell.add(up_triangle, down_triangle)

            triangle_cells.add(triangle_cell)
        
        self.play(
            LaggedStart(
                *[Create(triangles, run_time = 0.5) for triangles in triangle_cells],
                lag_ratio=0.005
            ),

            FadeOut(cpu),
            FadeOut(arrow),
            FadeOut(gpu)
        )

        self.wait(0.5)

        small_red_square = Square(cell_size * 1.75)
        small_red_square.set_color(RED)
        small_red_square.move_to(cells[0].get_center())

        big_red_square = Square(5)
        big_red_square.set_color(RED)
        big_red_square.shift(RIGHT * 3.5)

        line1 = Line(
            start = cells[0].get_center() + UP * h_cell_size * 1.75 + RIGHT * h_cell_size * 1.75,
            end = big_red_square.get_center() + LEFT * 2.5 + UP * 2.5
        )

        line2 = Line(
            start = cells[0].get_center() + DOWN * h_cell_size * 1.75 + RIGHT * h_cell_size * 1.75,
            end = big_red_square.get_center() + LEFT * 2.5 + DOWN * 2.5
        )

        line1.set_color(RED)
        line2.set_color(RED)

        self.play(Create(small_red_square), Create(big_red_square), Create(line1), Create(line2))

        self.wait(0.5)

        p1_dot = Dot(big_red_square.get_center() + UP * 1.5 + LEFT * 1.5, 0.13)
        p2_dot = Dot(big_red_square.get_center() + UP * 1.5 + RIGHT * 1.5, 0.13)
        p3_dot = Dot(big_red_square.get_center() + DOWN * 1.5 + LEFT * 1.5, 0.13)
        p4_dot = Dot(big_red_square.get_center() + DOWN * 1.5 + RIGHT * 1.5, 0.13)

        p2_dot2 = Dot(big_red_square.get_center() + UP * 1.5 + RIGHT * 1.5, 0.13)
        p3_dot2 = Dot(big_red_square.get_center() + DOWN * 1.5 + LEFT * 1.5, 0.13)

        self.play(
            GrowFromCenter(p1_dot), GrowFromCenter(p2_dot), 
            GrowFromCenter(p3_dot), GrowFromCenter(p4_dot)
        )

        self.wait(0.5)

        line_up = Line(p1_dot.get_center(), p2_dot.get_center()).set_stroke(width = 6.0)
        line_right = Line(p2_dot.get_center(), p4_dot.get_center()).set_stroke(width = 6.0)
        line_down = Line(p4_dot.get_center(), p3_dot.get_center()).set_stroke(width = 6.0)
        line_left = Line(p3_dot.get_center(), p1_dot.get_center()).set_stroke(width = 6.0)
        line_diag = Line(p3_dot.get_center(), p2_dot.get_center()).set_stroke(width = 6.0)
        line_diag2 = Line(p3_dot.get_center(), p2_dot.get_center()).set_stroke(width = 6.0)

        self.play(
            Create(line_up), Create(line_right), 
            Create(line_down), Create(line_left), 
            Create(line_diag), Create(line_diag2)
        )

        triangle1 = VGroup(p1_dot, p2_dot, p3_dot, line_up, line_diag, line_left)
        triangle2 = VGroup(p2_dot2, p3_dot2, p4_dot, line_right, line_diag2, line_down)

        triangle1.z_index = 1
        triangle2.z_index = 1

        self.play(
            triangle1.animate.shift(UP * 0.5 + LEFT * 0.5),
            triangle2.animate.shift(DOWN * 0.5 + RIGHT * 0.5),
            rate_func = rate_functions.ease_out_quart
        )

        self.play(
            triangle1.animate.shift(DOWN * 0.5 + RIGHT * 0.5),
            triangle2.animate.shift(UP * 0.5 + LEFT * 0.5),
            run_time = 0.2
        )

        self.wait(0.5)

        tri_poly1 = Polygon(p1_dot.get_center(), p2_dot.get_center(), p3_dot.get_center())
        tri_poly2 = Polygon(p2_dot.get_center(), p4_dot.get_center(), p3_dot.get_center())

        tri_poly1.set_stroke(opacity = 0.0).set_fill(YELLOW_D, 1.0)
        tri_poly2.set_stroke(opacity = 0.0).set_fill(YELLOW_D, 1.0)

        self.play(
            FadeIn(tri_poly1),
            FadeIn(tri_poly2)
        )

        self.wait(0.5)

        self.play(
            FadeOut(p1_dot), FadeOut(p2_dot),
            FadeOut(p3_dot), FadeOut(p4_dot),
            FadeOut(p2_dot2), FadeOut(p3_dot2),
            FadeOut(line_up), FadeOut(line_right),
            FadeOut(line_down), FadeOut(line_left),
            FadeOut(line_diag), FadeOut(line_diag2),
            FadeOut(tri_poly1), FadeOut(tri_poly2)
        )

        self.play(
            Uncreate(small_red_square),
            Uncreate(big_red_square),
            Uncreate(line1),
            Uncreate(line2)
        )

        self.wait(0.5)

        self.play(
            FadeIn(cpu),
            FadeIn(arrow),
            FadeIn(gpu)
        )

        self.wait(0.5)

        file.next_to(cpu, UP)
        self.play(FadeIn(file, run_time = 0.5))
        self.play(file.animate.shift(offset), run_time = 0.5)
        self.play(FadeOut(file, run_time = 0.5))
        self.wait(0.5)

        self.play(
            LaggedStart(
                *[Uncreate(triangles, run_time = 0.5) for triangles in reversed(triangle_cells)],
                lag_ratio=0.005
            ),

            FadeOut(cpu),
            FadeOut(arrow),
            FadeOut(gpu)
        )

        self.wait(0.5)
        
