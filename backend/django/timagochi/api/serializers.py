from rest_framework import serializers
from .models import Course, Player, InfiniteScore

class PlayerSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model=Player
        fields=('name', 'health', 'hunger', 'major', 'minor', 'happiness', 'gpa', 'gpa_slider', 'units', 'activity', 'last_seen')

class CourseSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model=Course
        fields=('person', 'course')


class InfiniteScoreSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model=InfiniteScore
        fields=('person', 'score')
