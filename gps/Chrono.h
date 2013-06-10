class Chrono {
    private:
        int _running;

    public:
        Chrono();
        void reset();
        void start();
        void stop();
        void toggle();
        bool isRunning();
};
