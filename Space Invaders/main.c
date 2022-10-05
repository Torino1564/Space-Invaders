#include <allegro5/allegro.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>

int main()
{
	al_init();
	al_install_keyboard();
	al_install_mouse();

	typedef union ALLEGRO_EVENT ALLEGRO_EVENT;


	typedef struct ALLEGRO_DISPLAY ALLEGRO_DISPLAY;
	ALLEGRO_DISPLAY* DISPLAY = al_create_display(1366, 768);

	typedef struct ALLEGRO_EVENT_SOURCE ALLEGRO_EVENT_SOURCE;
	typedef struct ALLEGRO_EVENT_QUEUE ALLEGRO_EVENT_QUEUE;

	ALLEGRO_EVENT_SOURCE* KeyboardEventSource = al_get_keyboard_event_source();

	ALLEGRO_EVENT_SOURCE* MouseEventSource = al_get_mouse_event_source();

	ALLEGRO_EVENT_QUEUE* InputEventQueue = al_create_event_queue();
	al_register_event_source(InputEventQueue, KeyboardEventSource);
	al_register_event_source(InputEventQueue, MouseEventSource);

	int run = 1;

	ALLEGRO_EVENT* TempEvent = malloc(sizeof(ALLEGRO_EVENT));
	if (!(TempEvent == NULL))
	{
		while (run)
		{
			if (!al_is_event_queue_empty(InputEventQueue))
			{
				al_get_next_event(InputEventQueue, TempEvent);
				if (TempEvent->any.source == KeyboardEventSource)
				{
					switch (TempEvent->keyboard.keycode)
					{
					case ALLEGRO_KEY_A:	
						al_clear_to_color(al_map_rgb(255, 0, 0));
						break;
					case ALLEGRO_KEY_B:
						al_clear_to_color(al_map_rgb(0, 255, 0));
						break;
					case ALLEGRO_KEY_C:
						al_clear_to_color(al_map_rgb(0, 0, 255));
						break;
					case ALLEGRO_KEY_Z:
						run = 0;
						break;
					default:
						break;
					}
				}
				if (TempEvent->any.source == MouseEventSource)
				{
					if (TempEvent->type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
					{
						switch (TempEvent->mouse.button)
						{
							case 1:
								al_clear_to_color(al_map_rgb(125, 125, 0));
								break;
							case 2:
								al_clear_to_color(al_map_rgb(125, 0, 125));
								break;
							default:
								break;
						}
					}
				}
			}


			al_flip_display();
		}
	}
	



	al_destroy_display(DISPLAY);
	return 0;
}