#ifndef GAME_A_STAR_HPP
#define GAME_A_STAR_HPP

#include <base.h>
#include <math/integer.h>


enum a_star_move {
    A_NULL = 0, A_ML, A_MU, A_MD, A_MR,
    P_NULL,     P_ML, P_MU, P_MD, P_MR,
};


bool a_star(context *ctx, game_state *gs,
            int x0, int y0, int x1, int y1,
            a_star_move *result, int result_size,
            bool32 draw = false)
{
    float32 g_cost[5][5] = {};
    float32 h_cost[5][5] = {};
    float32 f_cost[5][5] = {};
    bool32 visited[5][5] = {};
    a_star_move moves[5][5] = {};

    int i0 = x0 + 2;
    int j0 = y0 + 2;

    int i1 = x1 + 2;
    int j1 = y1 + 2;

    for (int i = 0; i < 5; i++) for (int j = 0; j < 5; j++)
    {
        g_cost[i][j] = infinity;
        h_cost[i][j] = (float32) (absolute(i - i1) + absolute(j - j1));
        f_cost[i][j] = infinity;
    }

    int shifts[4][2] = {
        {  1,  0 },
        { -1,  0 },
        {  0,  1 },
        {  0, -1 },
    };
    int shiftm[4] = {
        A_MR,
        A_ML,
        A_MU,
        A_MD,
    };

    g_cost[i0][j0] = 0.f;
    f_cost[i0][j0] = h_cost[i0][j0];

    while (true)
    {
        // Stop the loop when target node is reached
        if (f_cost[i1][j1] < infinity)
            break;

        int min_i = 0;
        int min_j = 0;
        float32 min_cost = infinity;

        // First, find the minimum of f_cost amongst all tiles
        for (int i = 0; i < 5; i++) for (int j = 0; j < 5; j++)
        {
            if (visited[i][j]) continue;
            if (f_cost[i][j] < min_cost)
            {
                min_i = i;
                min_j = j;
                min_cost = f_cost[i][j];
            }
        }

        // Since we set f_cost[i0][j0] < infinity, the condition is redundant
        if (min_cost < infinity)
        {
            visited[min_i][min_j] = true;
            for (int shift_index = 0; shift_index < 4; shift_index++)
            {
                int shift_x = shifts[shift_index][0];
                int shift_y = shifts[shift_index][1];

                int i = min_i + shift_x;
                int j = min_j + shift_y;

                int x = i - 2;
                int y = j - 2;

                if ((game::cell_is_empty(gs, x, y) ||
                    (i == i1 && j == j1)))
                {
                    float32 new_cost = g_cost[min_i][min_j] + 1.f;
                    if (new_cost < g_cost[i][j])
                    {
                        g_cost[i][j] = new_cost;
                        f_cost[i][j] = new_cost + h_cost[i][j];
                        visited[i][j] = false;
                        if (i < min_i) moves[i][j] = A_MR;
                        if (i > min_i) moves[i][j] = A_ML;
                        if (j < min_j) moves[i][j] = A_MU;
                        if (j > min_j) moves[i][j] = A_MD;
                    }
                }
            }
        }
        else
        {
            return false;
        }
    }

    {
        int i = i1;
        int j = j1;
        auto mv = moves[i][j];
        moves[i][j] = P_NULL;

        while (i != i0 || j != j0)
        {
            if (mv == A_MR)
            {
                // [->][mv?]
                mv = moves[i + 1][j];
                moves[i + 1][j] = P_ML;
                i = i + 1;
            }
            else if (mv == A_ML)
            {
                // [mv?][<-]
                mv = moves[i - 1][j];
                moves[i - 1][j] = P_MR;
                i = i - 1;
            }
            else if (mv == A_MU)
            {
                // [mv?]
                // [^^^]
                mv = moves[i][j + 1];
                moves[i][j + 1] = P_MD;
                j = j + 1;
            }
            else if (mv == A_MD)
            {
                // [vvv]
                // [mv?]
                mv = moves[i][j - 1];
                moves[i][j - 1] = P_MU;
                j = j - 1;
            }
            else
            {
                ASSERT_FAIL("Should not be possible");
            }
        }
    }

    {
        int i = i0;
        int j = j0;
        while (result_size > 0)
        {
            *result = moves[i][j];
            result += 1;
            result_size -= 1;
            if (moves[i][j] == P_ML) i -= 1;
            if (moves[i][j] == P_MR) i += 1;
            if (moves[i][j] == P_MU) j += 1;
            if (moves[i][j] == P_MD) j -= 1;
        }
    }

    // Render pathfinding debug
    if (draw)
    {
        for (int x = 0; x < 5; x++) for (int y = 0; y < 5; y++)
        {
            float32 cost = clamp(f_cost[x][y], 0.f, 25.f);
            cost = cvt(cost, 0.f, 25.f, 0.f, 1.f);
            auto color = V4(cost, 1.f - cost, 0.f, 1.f);
            if (moves[x][y] >= P_NULL) color = V4(0.4, 0.4, 0.8, 1);

            ctx->render_ui(
                           matrix4::translate((float32) x * 25.f + 600.f,
                                              (float32) (5 - y) * 25.f + 10.f, 0.f) *
                           matrix4::scale(10, 10, 1),
                           color);
        }
    }

    return true;
}



#endif // GAME_A_STAR_HPP
