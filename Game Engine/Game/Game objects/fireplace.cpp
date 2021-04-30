
#define FIREPLACE_SIZE 0.4

#define FIREPLACE_DELTA_DRAW_POS dot(-42, 10) * FIREPLACE_SIZE

#define FIREPLACE_COLLISION_RADIUS 10

#define FIREPLACE_LOG_EATE_RADIUS 20

#define FIREPLACE_LOG_ADD_TIME 1.5 * 60

struct Fireplace {

	dot pos;

	Fire_machine fire;

	point_t time = 5 * 60;
	point_t cooldown_add_time_accum = 0;

	Fireplace(){}
	Fireplace(const dot& p) {
		pos = p;
		fire = Fire_machine(pos, 5, 0.5, 0.5, 4, 0.15);
	}
	Fireplace(const char* memory) {
		
#define read(obj, type)\
obj = *reinterpret_cast<const type*>(memory);\
memory += sizeof(type);

		read(pos, dot);

		fire = Fire_machine(memory);
		memory += fire.get_mem_len();

		read(time, point_t);
		read(cooldown_add_time_accum, point_t);

#undef write
	}

	collision_circle get_collision() const {
		return Circle(pos + dot(0, -10) * FIREPLACE_SIZE, FIREPLACE_COLLISION_RADIUS);
	}

	void simulate(point_t delta_time) {
		/*if (rain.is_full_raining()) {
			time -= delta_time;
		}*/
		time -= delta_time;

		cooldown_add_time_accum += delta_time;

		if (cooldown_add_time_accum > 1) {
			fire.cooldown_add += 0.0005;
			cooldown_add_time_accum = 0;
		}

		fire.simulate(delta_time);

		for (s32 i = 0; i < Logs.size(); i++) {
			if ((pos - Logs[i].pos).getLen() <= FIREPLACE_LOG_EATE_RADIUS) {
				Logs.erase(Logs.begin() + i);
				i--;

				time += FIREPLACE_LOG_ADD_TIME;
				fire.cooldown_add = std::max(0.3, fire.cooldown_add - 0.3);
			}
		}
	}

	void draw() const {

		draw_sprite(pos + FIREPLACE_DELTA_DRAW_POS + dot(10, 0) * FIREPLACE_SIZE, FIREPLACE_SIZE * 2, SP_LARGE_SHADOW);
		draw_sprite(pos + FIREPLACE_DELTA_DRAW_POS, FIREPLACE_SIZE, SP_FIREPLACE);

		fire.draw();


		dot p = pos + dot(-10, -30) * FIREPLACE_SIZE;
		static_pos_update(p);
		draw_object(s32(time), p, 0.5, 0xff000000);

		
		draw_collision_obj(*this);
	}

	u32 get_mem_len() {
		u32 res = fire.get_mem_len();
		
		res += sizeof(pos);

		res += sizeof(time);
		res += sizeof(cooldown_add_time_accum);

		return res;
	}

	// заполняет указатель данной структурой
	void fill_mem(char* memory) {

#define write(obj, type)\
*reinterpret_cast<type*>(memory) = obj;\
memory += sizeof(type);

		write(pos, dot);

		fire.fill_mem(memory);
		memory += fire.get_mem_len();

		write(time, point_t);
		write(cooldown_add_time_accum, point_t);

#undef write
	}
};

