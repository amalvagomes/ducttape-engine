
// ----------------------------------------------------------------------------
// This file is part of the Ducttape Project (http://ducttape-dev.org) and is
// licensed under the GNU LESSER PUBLIC LICENSE version 3. For the full license
// text, please see the LICENSE file in the root of this project or at
// http://www.gnu.org/licenses/lgpl.html
// ----------------------------------------------------------------------------

#include "FollowPathComponent.hpp"

#include "scene/Node.hpp"

#include <boost/foreach.hpp>

namespace dt {

FollowPathComponent::FollowPathComponent(const std::string& name)
    : Component(name) {
    mSmoothAcceleration = false;
    mSmoothCorners = 0;
}

void FollowPathComponent::HandleEvent(std::shared_ptr<Event> e) {}

void FollowPathComponent::OnCreate() {}
void FollowPathComponent::OnDestroy() {}

void FollowPathComponent::OnUpdate(double time_diff) {
    if(IsEnabled()) {
        // move progress further
        mDurationSinceStart += time_diff;

        // calculate new position
        if(mNode != nullptr)
            mNode->SetPosition(_CalculatePosition());
    }
}

void FollowPathComponent::AddPoint(Ogre::Vector3 point) {
    mPoints.push_back(point);
    if(mPoints.size() == 1) {
        Reset();
    }
}

void FollowPathComponent::SetSpeed(float speed) {
    mTotalDuration = GetTotalLength() / speed;
}

void FollowPathComponent::SetDuration(float duration) {
    mTotalDuration = duration;
}

float FollowPathComponent::GetTotalLength() const {
    if(mPoints.size() <= 1)
        return 0;

    float total = 0;
    const Ogre::Vector3* last = &mPoints[0];
    for(auto iter = mPoints.begin() + 1; iter != mPoints.end(); ++iter) {
        total += (*last - *iter).length();
        last = &(*iter);
    }
    return total;
}

void FollowPathComponent::Reset() {
    mDurationSinceStart = 0;
    if(mPoints.size() > 0 && mNode != nullptr) {
        mNode->SetPosition(mPoints[0]);
    }
}

Ogre::Vector3 FollowPathComponent::_CalculatePosition() {
    if(mPoints.size() == 0) {
        if(mNode != nullptr)
            return mNode->GetPosition();
        else
            return Ogre::Vector3::ZERO;
    }

    if(mPoints.size() == 1) {
        return mPoints[0];
    }

    float length_travelled = mDurationSinceStart / mTotalDuration * GetTotalLength();
    float total = 0;
    Ogre::Vector3* last = & mPoints[0];
    for(std::vector<Ogre::Vector3>::iterator iter = mPoints.begin() + 1; iter != mPoints.end(); ++iter) {
        float segment_length = (*last - *iter).length();
        if(total <= length_travelled && length_travelled <= total + segment_length) {
            // we're in the right segment
            float segment_progress = (length_travelled - total) / segment_length;

            //std::cout << *iter << std::endl;
            if(!mSmoothCorners ||
                    (iter == mPoints.begin() + 1 && segment_progress < 0.5) ||         // we're in the first part of the first segment
                    (iter == mPoints.end() && segment_progress >= (1.0 - 0.5) ) ) {    // we're in the last part of the last segment
                if(mSmoothAcceleration)
                    segment_progress = Math::SmoothStep(0, 1, segment_progress); // 0.5 will still be 0.5 :D

                return (*last) + ((*iter)- (*last)) * segment_progress;
            } else {
                // this is gonna be complicated ^^

                // determine the corner we are at
                std::vector<Ogre::Vector3>::iterator corner = iter - 1; // current corner is begind
                if(segment_progress >= 0.5) {
                    corner = iter;                                  // current corner is behind
                }
                Ogre::Vector3 c = *corner;
                Ogre::Vector3 n = *(corner+1);
                Ogre::Vector3 p = *(corner-1);

                float corner_progress = segment_progress - 0.5;
                if(segment_progress < 0.5)
                    corner_progress += 1.0;

                return c + (n - c) * 0.5 * Math::BounceIn(0, 1, corner_progress, 0)
                         + (p - c) * 0.5 * Math::BounceOut(1, 0, corner_progress, 0);

                // well done!
            }
        }

        total += segment_length;
        last = &(*iter);
    }
    return mPoints.back();
}

void FollowPathComponent::SetSmoothAcceleration(bool smooth_acceleration) {
    mSmoothAcceleration = smooth_acceleration;
}

bool FollowPathComponent::GetSmoothAcceleration() const {
    return mSmoothAcceleration;
}

void FollowPathComponent::SetSmoothCorners(bool smooth_corners) {
    mSmoothCorners = smooth_corners;
}

bool FollowPathComponent::GetSmoothCorners() const {
    return mSmoothCorners;
}

}
