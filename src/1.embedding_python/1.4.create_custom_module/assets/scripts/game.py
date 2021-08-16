import engine


class Player:
    def talk(self, message: str) -> None:
        engine_version = engine.get_version()
        engine.print_log(message=f"Engine version = {engine_version}")
