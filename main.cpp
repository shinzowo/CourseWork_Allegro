#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

struct Obstacle {
    float x, y, size;
};

struct Player {
    float x, y, size;
    float speed;
};

bool check_collision(const Player& player, const Obstacle& obstacle) {
    return player.x < obstacle.x + obstacle.size &&
           player.x + player.size > obstacle.x &&
           player.y < obstacle.y + obstacle.size &&
           player.y + player.size > obstacle.y;
}

void generate_obstacle(std::vector<Obstacle>& obstacles, int screen_width, int screen_height) {
    float x = rand() % (screen_width - 50);
    float y = rand() % (screen_height - 50);
    obstacles.push_back({x, y, 50});
}

int main() {
    // Инициализация Allegro
    al_init();
    al_install_keyboard();
    al_init_primitives_addon();
    srand(static_cast<unsigned>(time(0)));

    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_TIMER* obstacle_timer = al_create_timer(2.0); // Таймер для генерации препятствий
    ALLEGRO_FONT* font = al_create_builtin_font();

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_timer_event_source(obstacle_timer));

    // Создание игрока
    Player player = {100, 100, 30, 5};

    // Создание препятствий
    std::vector<Obstacle> obstacles;
    for (int i = 0; i < 10; ++i) {
        generate_obstacle(obstacles, SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    bool running = true;
    bool redraw = true;
    ALLEGRO_KEYBOARD_STATE key_state;

    al_start_timer(timer);
    al_start_timer(obstacle_timer);
    unsigned int points = 0;
    std::string points_str = std::to_string(points);
    while (running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        } else if (event.type == ALLEGRO_EVENT_TIMER) {
            if (event.timer.source == timer) {
                al_get_keyboard_state(&key_state);

                // Управление игроком
                if (al_key_down(&key_state, ALLEGRO_KEY_UP) && player.y > 0) {
                    player.y -= player.speed;
                }
                if (al_key_down(&key_state, ALLEGRO_KEY_DOWN) && player.y < SCREEN_HEIGHT - player.size) {
                    player.y += player.speed;
                }
                if (al_key_down(&key_state, ALLEGRO_KEY_LEFT) && player.x > 0) {
                    player.x -= player.speed;
                }
                if (al_key_down(&key_state, ALLEGRO_KEY_RIGHT) && player.x < SCREEN_WIDTH - player.size) {
                    player.x += player.speed;
                }

                // Проверка столкновений
                for (auto it = obstacles.begin(); it != obstacles.end(); ) {
                    if (check_collision(player, *it)) {
                        it = obstacles.erase(it); // Убираем препятствие
                        points++;
                    } else {
                        ++it;
                    }
                }

                redraw = true;
            } else if (event.timer.source == obstacle_timer) {
                // Генерация нового препятствия
                generate_obstacle(obstacles, SCREEN_WIDTH, SCREEN_HEIGHT);
            }
        }

        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;

            // Рисуем сцену
            al_clear_to_color(al_map_rgb(0, 0, 0));

            // Рисуем игрока
            al_draw_filled_rectangle(player.x, player.y, player.x + player.size, player.y + player.size, al_map_rgb(0, 255, 0));

            // Рисуем препятствия
            for (const auto& obstacle : obstacles) {
                al_draw_filled_rectangle(obstacle.x, obstacle.y, obstacle.x + obstacle.size, obstacle.y + obstacle.size, al_map_rgb(255, 0, 0));
            }

            // Рисуем заголовок
            points_str=std::to_string(points);
            al_draw_text(font, al_map_rgb(255, 255, 255), 30, 30, 0, points_str.c_str());
            al_flip_display();
        }
    }

    // Очистка ресурсов
    al_destroy_timer(timer);
    al_destroy_timer(obstacle_timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);

    return 0;
}
