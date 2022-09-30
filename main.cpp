// MiraiCP依赖文件(只需要引入这一个)
#include <MiraiCP.hpp>
#include "Suzuchan.h"

using namespace MiraiCP;

extern bool pyenv_ok;

// 插件实例
class Main : public CPPPlugin {
public:
    // 配置插件信息
    Main() : CPPPlugin(PluginConfig(
            "Suzuchan-3g", "Suzuchan 3G", Suzuchan::VERSION, "Suzuco Kitsuneko",
            "Suzuchan, the 3rd generation", Suzuchan::COMPILE_TIME))
    {}

    void onEnable() override
    {
        // 监听
        /*
        Event::processor.registerEvent<NudgeEvent>([](NudgeEvent e) -> void {
            Logger::logger.info("NudgeEvent from " + std::to_string(e.from.groupid()));
            if (likely(e.from.groupid() != 0)) {
                // NudgeEvent from group
                Member m(e.from.id(), e.from.groupid(), e.bot.id);
                m.sendNudge();
            }
            else {
                // NudgeEvent from user - unlikely!
                e.from.sendMessage("喵！");
            }
        });*/
        pyenv_ok = false;

        MiraiCP::Event::registerEvent<GroupTempMessageEvent>([](const GroupTempMessageEvent & e) -> void {});

        MiraiCP::Event::registerEvent<GroupMessageEvent>(& Suzuchan::process);

    }

    void onDisable() override
    {
        /*插件结束*/
    }
};

// 绑定当前插件实例
void MiraiCP::enrollPlugin()
{
    MiraiCP::enrollPlugin0(new Main());
}
