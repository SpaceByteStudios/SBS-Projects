from manim import *

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

