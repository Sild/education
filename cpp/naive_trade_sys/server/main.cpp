#include <iostream>
#include <cstdlib>
#include <functional>
#include <unordered_map>
#include <map>
#include <queue>

static constexpr uint64_t INSTRUMENTS_COUNT = 4;
static constexpr uint64_t EXECUTED_QUEUE_SIZE = 10000;

struct Order {
    Order(bool aBuy):
        m_ID(std::rand()),
        m_ClientID(std::rand() % 2000),
        m_InstrumentID(std::rand() % INSTRUMENTS_COUNT + 1),
        m_Price(std::rand() % 1000 + 1000),
        m_Count(std::rand() % 100),
        m_Buy(aBuy)
    {
    }
    uint64_t m_ID = 0;
    uint64_t m_ClientID = 0;
    uint64_t m_InstrumentID = 0;
    uint64_t m_Price = 0;
    uint64_t m_Count = 0;
    bool m_Buy = false;
    void print() {
        std::cout << "order: id=" << m_ID << ", cli_id=" << m_ClientID << ", ins_id=" << m_InstrumentID << ", price=" << m_Price << ", count=" << m_Count << std::endl;
    }
};

struct DOMOrder {
    uint64_t m_ID = 0;
    uint64_t m_Count = 0; 
};

struct ExecutedOrder {
    DOMOrder m_Sell;
    DOMOrder m_Buy;
};

class DOM {
    using ExecutedCallback = std::function<bool(ExecutedOrder&&)>;

    struct OrdersInfo {
        std::queue<DOMOrder> m_Orders;
        uint64_t m_SumCount = 0;
    };

    public:
        DOM(ExecutedCallback aExecCallback)
            : m_ExecutedCallback(aExecCallback) {
        }
        bool accept(const Order& aOrder) {
            if (aOrder.m_Price > m_LastDealPrice * 1.2 || aOrder.m_Price < m_LastDealPrice * 0.8) {
                std::cerr << "order price=" << aOrder.m_Price << " too far from last_price=" << m_LastDealPrice << std::endl;
                return false
            }
            if (m_OrdersCount > aOrder) {
                std::cerr << "queue for instrument_id=" << aOrder.m_InstrumentID << " is full" << std::endl;
                return false;
            }
            auto& sDst = aOrder.m_Buy ? m_Buy : m_Sell;
            auto& sOrdersInfo = sDst[aOrder.m_Price];
            sOrdersInfo.m_Orders.push(DOMOrder{aOrder.m_ID, aOrder.m_Count});
            sOrdersInfo.m_SumCount += aOrder.m_Count;
            m_OrdersCount++;
            return true;
        }
    private:
        std::map<int64_t, OrdersInfo> m_Sell;
        std::map<int64_t, OrdersInfo> m_Buy;
        std::atomic<uint64_t> m_OrdersCount = 0;
        ExecutedCallback m_ExecutedCallback;
        std::atomic<int64_t> m_LastDealPrice;
};

class Market {
    friend class DOM;
    public:
        Market(uint64_t aExecQueueSize) {
            auto execCallback = [this, aExecQueueSize](ExecutedOrder&& execOrder) {
                if (m_ExecutedOrders.size() == aExecQueueSize) {
                    return false;
                }
                m_ExecutedOrders.push(std::move(execOrder));
                return true;
            };
            for (int i = 0; i < INSTRUMENTS_COUNT; ++i) {
                m_DOMs.emplace(i, execCallback);
            }
        }
        bool accept(const Order& aOrder) {
            if (auto it = m_DOMs.find(aOrder.m_InstrumentID); it != m_DOMs.end()) {
                return it->second.accept(aOrder);
            }
            std::cerr << "Fail to find DOM for instrument_id=" << aOrder.m_InstrumentID << std::endl;
            return false;
        }
        bool getExecuted(ExecutedOrder& aExecOrder) {
            if (!m_ExecutedOrders.empty()) {
                aExecOrder = std::move(m_ExecutedOrders.front());
                m_ExecutedOrders.pop();
                return true;
            }
            return false;
        }
        
    private:
        std::unordered_map<uint64_t, DOM> m_DOMs;
        std::unordered_map<uint64_t, Order> m_OrderStore;
        std::queue<ExecutedOrder> m_ExecutedOrders;
};



int main() {
    std::srand((unsigned) time(NULL));

    auto market = Market(EXECUTED_QUEUE_SIZE);
    auto order = Order(/*buy=*/ false);
    order.print();
    if (!market.accept(order)) {
        std::cerr << "Market failed to accept order with id=" << order.m_ID << std::endl;
    } else {
        std::cout << "Market accept order with id=" << order.m_ID << std::endl;
    }
    return 0;
}